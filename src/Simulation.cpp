#include "Simulation.h"
#include "Window.h"
#include "Renderer.h"
#include "Interfaces.h"
#include "Map.h"
#include "Exceptions.h"
#include "Camera.h"
#include "QuadTree.h"

#include <MiniYaml/Yaml.hpp>

#include <string>
#include <vector>
#include <memory>

class GreenRenderable : public Ecosim::Renderable, public Ecosim::Simulatable
{
private:
    Ecosim::Color m_color;
    Ecosim::Vector2<int> m_center;
    Ecosim::Vector2<int> m_velocity;
    int m_radius;

public:
    GreenRenderable() : m_color(0, 200, 80), m_center(120, 200), m_velocity(0), m_radius(40) {}

    void Draw() override
    {
        Ecosim::Vector2<int> direction(m_velocity.x * m_radius + 10, m_velocity.y * m_radius + 10);
        Ecosim::Renderer::Line(m_center, m_center + direction, 2, m_color);
        Ecosim::Renderer::Circle(m_center, m_radius, m_color);
    }

    void Step() override
    {
        m_velocity.x = (std::rand() % 11) - 5;
        m_velocity.y = (std::rand() % 11) - 5;

        m_center.x += m_velocity.x;
        m_center.y += m_velocity.y;
    }
};

class RedRenderable : public Ecosim::Renderable, public Ecosim::Simulatable
{
private:
    Ecosim::Color m_color;
    Ecosim::Vector2<int> m_center;
    Ecosim::Vector2<int> m_velocity;
    int m_radius;

public:
    RedRenderable() : m_color(200, 80, 0), m_center(300, 200), m_velocity(0), m_radius(60) {}

    void Draw() override
    {
        Ecosim::Vector2<int> direction(m_velocity.x * m_radius + 10, m_velocity.y * m_radius + 10);
        Ecosim::Renderer::Line(m_center, m_center + direction, 2, m_color);
        Ecosim::Renderer::Circle(m_center, m_radius, m_color);
    }

    void Step() override
    {
        m_velocity.x = (std::rand() % 11) - 5;
        m_velocity.y = (std::rand() % 11) - 5;

        m_center.x += m_velocity.x;
        m_center.y += m_velocity.y;
    }
};

void handleKeyRelease(SDL_KeyboardEvent &keyEvent)
{
    Ecosim::Vector2<float> move{};
    switch (keyEvent.keysym.sym)
    {
    case SDLK_w:
        move.y = -50.0f;
        break;
    case SDLK_a:
        move.x = -50.0f;
        break;
    case SDLK_s:
        move.y = 50.0f;
        break;
    case SDLK_d:
        move.x = 50.0f;
        break;
    case SDLK_z:
        Ecosim::Camera::Zoom(0.2f);
        break;
    case SDLK_x:
        Ecosim::Camera::Zoom(-0.2f);
        break;
    }
    Ecosim::Camera::MovePosition(move);
}

namespace Ecosim
{
    SimulationConfig::SimulationConfig(std::string &configPath) { SimulationConfig(configPath.c_str()); }
    SimulationConfig::SimulationConfig(const char *configPath)
    {
        Yaml::Node root;
        try
        {
            Yaml::Parse(root, configPath);
        }
        ECOSIM_CATCH_AND_CALL(Yaml::ParsingException & e, SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Failed to parse yaml source: '%s'\n%s", configPath, e.what()))
        ECOSIM_CATCH_AND_CALL(Yaml::OperationException & e, SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Invalid filepath to yaml source: '%s'", configPath))
        ECOSIM_CATCH_AND_CALL(std::exception & e, SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Failed to open and read yaml source: '%s'\n%s", configPath, e.what()))

        name = root["simulation-name"].As<std::string>("Ecosim");
        numAgents = root["num-agents"].As<uint>(100);
        numFood = root["total-food"].As<uint>(100);
        enviromentConfigPath = root["enviroment"].As<std::string>("<no enviroment>");
        numSteps = root["num-steps"].As<uint>(100);
        storeData = root["store-data"].As<bool>(false);
    }

    Simulation::Simulation(std::string &configPath) { Simulation(configPath.c_str()); }
    Simulation::Simulation(const char *configPath)
    {
        m_config = SimulationConfig(configPath);
        m_window = Window(m_config.name.c_str(), 800, 800);

        Camera::SetViewport(Vector2<float>(800.0f, 800.0f));
        Camera::SetZoom(0.25f);

        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "Started simulation: '%s'\n  - Number of agents: %d\n  - Number of food:   %d\n  - Enviroment:       '%s'",
                    m_config.name.c_str(), m_config.numAgents, m_config.numFood, m_config.enviromentConfigPath.c_str());

        Map::Create(m_config.enviromentConfigPath);
    }

    void Simulation::Simulate()
    {
        std::shared_ptr<GreenRenderable> obj1 = std::make_shared<GreenRenderable>();
        std::shared_ptr<RedRenderable> obj2 = std::make_shared<RedRenderable>();

        std::vector<std::shared_ptr<Renderable>> renderables;
        renderables.push_back(obj1);
        renderables.push_back(obj2);

        std::vector<std::shared_ptr<Simulatable>> simulatables;
        simulatables.push_back(obj1);
        simulatables.push_back(obj2);

        SDL_Event sdlEvent;
        bool shouldClose = false;

        // Quad tree testing. Slet senere, når alt virker.
        std::vector<Vector2<int>> points;
        for (int i = 0; i < 1000; ++i)
            points.push_back(Vector2<int>(std::rand() % 800, std::rand() % 800));

        Node boundary(Vector2<int>(400, 400), 800);
        QuadTree qt(boundary, 2);

        for (const auto &point : points)
            qt.Insert(point);

        // End of quad tree testing. Slet senere, når alt virker.

        while (!shouldClose)
        {
            while (SDL_PollEvent(&sdlEvent))
            {
                switch (sdlEvent.type)
                {
                case SDL_EVENT_QUIT:
                    shouldClose = true;
                    break;
                case SDL_EVENT_KEY_UP:
                    handleKeyRelease(sdlEvent.key);
                    break;
                }
            }

            for (const auto &simulatable : simulatables)
                simulatable->Step(/*delta time*/);

            Renderer::Background(Color(51, 77, 102));
            Map::Render();
            Renderer::Circle(80, 100, 50, Color(180, 80, 150));
            for (const auto &point : points)
                Renderer::Rect(point, Vector2<int>(2, 2), Color(255, 255, 255));

            for (const auto &renderable : renderables)
                renderable->Draw();

            qt.Render();

            Renderer::RenderFrame();
        }

        Map::Cleanup();
    }
}
#include "Simulation.h"
#include "Window.h"
#include "Renderer.h"
#include "Interfaces.h"
#include "Map.h"

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
        // Ecosim::Renderer::Line(m_center, m_center + direction, m_color);
        Ecosim::Renderer::Line(m_center, m_center + direction, 2, m_color);
        Ecosim::Renderer::Circle(m_center, m_radius, m_color);
    }

    void Step() override
    {
        m_velocity.x = (std::rand() % 10) - 5;
        m_velocity.y = (std::rand() % 10) - 5;

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
        m_velocity.x = (std::rand() % 10) - 5;
        m_velocity.y = (std::rand() % 10) - 5;

        m_center.x += m_velocity.x;
        m_center.y += m_velocity.y;
    }
};

namespace Ecosim
{
    Simulation::Simulation(const char *configPath)
    {
        Yaml::Node root;
        Yaml::Parse(root, configPath);

        std::string windowTitle = root["simulation-name"].As<std::string>("Ecosim");
        m_window = Window(windowTitle.c_str(), 800, 800);

        m_numAgents = root["num-agents"].As<int>(100);
        m_numFood = root["total-food"].As<int>(100);
        m_enviromentConfig = root["enviroment"].As<std::string>("<no enviroment>");

        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "Started simulation: '%s'\n  - Number of agents: %d\n  - Number of food:   %d\n  - Enviroment:       '%s'",
                    windowTitle.c_str(), m_numAgents, m_numFood, m_enviromentConfig.c_str());

        Map::Create(m_enviromentConfig.c_str());
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

        while (!shouldClose)
        {
            while (SDL_PollEvent(&sdlEvent))
            {
                if (sdlEvent.type == SDL_EVENT_QUIT)
                    shouldClose = true;
            }

            for (const auto &simulatable : simulatables)
                simulatable->Step(/*delta time*/);

            Renderer::Background(Color(0, 255, 0));
            Map::Render();

            for (const auto &renderable : renderables)
                renderable->Draw();

            Renderer::RenderFrame();
        }

        Map::Cleanup();
    }
}
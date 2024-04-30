#include "Simulation.h"
#include "Window.h"
#include "Renderer.h"
#include "Interfaces.h"
#include "Map.h"
#include "Exceptions.h"
#include "Camera.h"
#include "Agent.h"
#include "QuadTree.h"
#include "Food.h"

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
        std::vector<std::shared_ptr<Renderable>> renderables;
        std::vector<std::shared_ptr<Simulatable>> simulatables;
        std::vector<std::shared_ptr<Collidable>> collidables;

        for (int i = 0; i < 100; ++i)
        {
            std::shared_ptr<Agent> agent = std::make_shared<Agent>(Vector2<float>(std::rand() % 800, std::rand() % 800));
            renderables.push_back(agent);
            simulatables.push_back(agent);
            collidables.push_back(agent);
        }

        // for (int i = 0; i < 300; ++i)
        // {
        //     std::shared_ptr<Food> food = std::make_shared<Food>(Vector2<float>(std::rand() % 800, std::rand() % 800));
        //     renderables.push_back(food);
        //     collidables.push_back(food);
        // }

        SDL_Event sdlEvent;
        bool shouldClose = false;

        // Quad tree testing. Slet senere, når alt virker.

        Node root(Vector2<int>(400, 400), 800);
        Node range(Vector2<int>(400, 400), 800);

        int count = 0;
        for (auto &collidable : collidables)
        {
            if (root.Contains(collidable->getPos()))
                count++;
        }

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Number of agents in root: %d", count);
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Does root and range intersect?: %d", root.IntersectsNode(range));

        QuadTree<Collidable *> quadTree(root, 1);

        for (auto &collidable : collidables)
            quadTree.Insert(collidable.get());

        std::vector<Collidable *> found;
        quadTree.Query(root, found);

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Number of agents in range: %d", found.size());
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
            for (const auto &renderable : renderables)
                renderable->Draw();

            // quadTree.Clear();

            // for (auto &collidable : collidables)
            //     quadTree.Insert(collidable.get());

            // for (auto &collidable : collidables)
            // {
            //     std::vector<Collidable *> objects;
            //     Node test(collidable->getPos().Convert<int>(), 10);
            //     quadTree.Query(test, objects);

            //     for (auto &other : objects)
            //     {
            //         if (collidable->Collides(*other))
            //         {
            //             if (auto agent = dynamic_cast<Agent *>(collidable.get()))
            //             {
            //                 if (auto food = dynamic_cast<Food *>(other))
            //                 {
            //                     // agent->Eat(food->OnEaten(*agent));
            //                     food->ResetFood();
            //                 }
            //             }
            //         }
            //     }
            // }

            // root.Render();
            // range.Render();

            // quadTree.Render();

            Renderer::RenderFrame();
        }

        Map::Cleanup();
    }
}

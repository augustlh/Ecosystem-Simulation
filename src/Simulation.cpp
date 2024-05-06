#include "Ecosim.h"
#include "Simulation.h"
#include "Window.h"
#include "Renderer.h"
#include "Interfaces.h"
#include "Map.h"
#include "Camera.h"
#include "Agent.h"
#include "CollisionHandler.h"
#include "Food.h"

#include <MiniYaml/Yaml.hpp>

#include <string>
#include <vector>
#include <memory>

#include "Removeme_AgentStuff.h"
#include "Statistics.h"

void HandleKeyRelease(SDL_KeyboardEvent &keyEvent)
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

        name = root["simulation-name"].As<std::string>("Unnamed simulation");
        numAgents = root["num-agents"].As<uint>(100);
        numFood = root["total-food"].As<uint>(200);
        enviromentConfigPath = root["enviroment"].As<std::string>("<no enviroment>");
        numSeconds = root["num-seconds"].As<uint>(0);
        storeData = root["store-data"].As<bool>(false);
    }

    Simulation::Simulation(const char *configPath)
    {
        m_config = SimulationConfig(configPath);
        m_window = Window(m_config.name.c_str(), 800, 800);

        Camera::SetViewport(Vector2<float>(800.0f, 800.0f));
        Camera::SetZoom(0.25f);

        Map::Create(m_config.enviromentConfigPath);

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "Started simulation: '%s'\n  - Number of agents: %d\n  - Number of food:   %d\n  - Enviroment:       '%s'\n  - Runtime (sec):    %d\n  - Store data:       %s",
                    m_config.name.c_str(), m_config.numAgents, m_config.numFood, m_config.enviromentConfigPath.c_str(), m_config.numSeconds, m_config.storeData ? "true" : "false");
    }

    void Simulation::Simulate(uint fps)
    {
        uint64_t thisFrame = SDL_GetTicks(), lastFrame = 0;
        uint64_t initFrame = thisFrame;

        bool limitDisplay = fps > 0;
        bool limitRuntime = m_config.numSeconds > 0;

        uint64_t lastDisplayed = 0;
        uint64_t displayInterval = limitDisplay ? 1000 / fps : 0;

        SDL_Event sdlEvent;
        bool shouldClose = false;

        std::vector<std::shared_ptr<Renderable>> renderables;
        std::vector<std::shared_ptr<Simulatable>> simulatables;
        std::vector<std::shared_ptr<Collidable>> collidables;

        Statistics::Initalize();

        for (int i = 0; i < m_config.numAgents; ++i)
        {
            Agent a{};
            std::shared_ptr<Agent> ptr = std::make_shared<Agent>(a);

            renderables.emplace_back(ptr);
            simulatables.emplace_back(ptr);
            collidables.emplace_back(ptr);
        }

        for (int i = 0; i < m_config.numFood; ++i)
        {
            Food f{};
            std::shared_ptr<Food> ptr = std::make_shared<Food>(f);

            renderables.emplace_back(ptr);
            collidables.emplace_back(ptr);

            Statistics::Report("Food_X", f.getPosition().x);
            Statistics::Report("Food_Y", f.getPosition().y);
        }

        CollisionHandler::SetCollidables(collidables);

        while (!shouldClose)
        {
            while (SDL_PollEvent(&sdlEvent))
            {
                switch (sdlEvent.type)
                {
                case SDL_EVENT_QUIT:
                    shouldClose = true;
                    break;
                case SDL_EVENT_KEY_UP: // FIXME:
                    HandleKeyRelease(sdlEvent.key);
                    break;
                }
            }

            lastFrame = thisFrame;
            thisFrame = SDL_GetTicks();

            if (limitRuntime && (thisFrame - initFrame) * 0.001 >= m_config.numSeconds)
                shouldClose = true;

            double deltaTime = (thisFrame - lastFrame) * 0.001;

            for (const auto &simulatable : simulatables)
                simulatable->Step(deltaTime);

            if (!limitDisplay || thisFrame - lastDisplayed >= displayInterval)
            {
                lastDisplayed = thisFrame;

                Renderer::Background(Color(51, 77, 102));
                Map::Render();

                // float mouseX, mouseY;
                // SDL_GetMouseState(&mouseX, &mouseY);
                // Vector2<int> coord = Camera::ViewportToWorld(Vector2<int>(mouseX, mouseY));
                // bool water = Map::WaterAt(coord);
                // BiomeType biome = Map::BiomeAt(coord);
                // std::cout << "Mouse hovers over: water=" << water << ", biome=" << biome.name << ", coord=" << coord.x << "," << coord.y << std::endl;

                for (const auto &renderable : renderables)
                    renderable->Draw();

                CollisionHandler::Render();

                Renderer::RenderFrame();
            }

            CollisionHandler::SetCollidables(collidables);
            CollisionHandler::Rebuild();
            CollisionHandler::CheckCollisions();
        }

        Map::Cleanup();

        if (m_config.storeData)
            Statistics::Export("FoodData");
    }
}
#include "Simulation.h"
#include "Window.h"
#include "Renderer.h"
#include "Interfaces.h"
#include "Map.h"
#include "Exceptions.h"
#include "Camera.h"

#include <MiniYaml/Yaml.hpp>

#include <string>
#include <vector>
#include <memory>

#include "AgentStuff.h"
#include "Statistics.h"

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

    Simulation::Simulation(const char *configPath)
    {
        m_config = SimulationConfig(configPath);
        m_window = Window(m_config.name.c_str(), 800, 800);

        Camera::SetViewport(Vector2<float>(800.0f, 800.0f));
        Camera::SetZoom(0.25f);

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "Started simulation: '%s'\n  - Number of agents: %d\n  - Number of food:   %d\n  - Enviroment:       '%s'",
                    m_config.name.c_str(), m_config.numAgents, m_config.numFood, m_config.enviromentConfigPath.c_str());

        Map::Create(m_config.enviromentConfigPath);
    }

    void Simulation::Simulate(uint fps)
    {
        std::vector<std::shared_ptr<Renderable>> renderables;
        std::vector<std::shared_ptr<Simulatable>> simulatables;

        for (int i = 0; i < 100; ++i)
        {
            Temp::Agent a{};
            std::shared_ptr<Temp::Agent> ptr = std::make_shared<Temp::Agent>(a);

            renderables.emplace_back(ptr);
            simulatables.emplace_back(ptr);
        }

        for (int i = 0; i < 100; ++i)
        {
            Temp::Food f{};
            renderables.emplace_back(std::make_shared<Temp::Food>(f));

            Statistics::Report("Food_X", f.pos.x);
            Statistics::Report("Food_Y", f.pos.y);

            // if (Statistics::MemorySize() >= 200)
            //     Statistics::Export("FoodData");
        }

        uint64_t thisFrame = SDL_GetTicks(), lastFrame = 0;

        bool limitDisplay = fps > 0;

        uint64_t lastDisplayed = 0;
        uint64_t displayInterval = limitDisplay ? 1000 / fps : 0;

        SDL_Event sdlEvent;
        bool shouldClose = false;

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
                    handleKeyRelease(sdlEvent.key);
                    break;
                }
            }

            lastFrame = thisFrame;
            thisFrame = SDL_GetTicks();

            double deltaTime = (thisFrame - lastFrame) * 0.001;

            for (const auto &simulatable : simulatables)
                simulatable->Step(deltaTime);

            if (!limitDisplay || thisFrame - lastDisplayed >= displayInterval)
            {
                lastDisplayed = thisFrame;

                Renderer::Background(Color(51, 77, 102));
                Map::Render();
                Renderer::Circle(80, 100, 50, Color(180, 80, 150));

                for (const auto &renderable : renderables)
                    renderable->Draw();

                Renderer::RenderFrame();
            }
        }

        Map::Cleanup();
        // Statistics::Export("FoodData");
    }
}
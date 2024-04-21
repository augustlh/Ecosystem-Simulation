#include "Simulation.h"
#include <iostream>

int main(int argc, char *argv[])
{
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    const char *configPath = "<no config path>";
    int renderFPS = 24;

    try
    {
        if (argc > 1)
            configPath = argv[1];
        if (argc > 2)
            renderFPS = std::stoi(argv[2]);
        if (argc > 3)
        {
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Incorrect usage, use:\n%s config.yaml [fps=24]\nSee 'README.md' for more information", argv[0]);
            return -1;
        }

        Ecosim::Simulation simulation(configPath);
        simulation.Simulate(renderFPS);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error:" << e.what() << std::endl;
        return -1;
    }

    return 0;
}

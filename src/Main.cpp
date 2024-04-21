#include "Simulation.h"
#include <iostream>

int main(int argc, char *argv[])
{
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    if (argc < 2)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Incorrect usage, use:\n%s config.yaml [fps=24]\nSee 'README.md' for more information", argv[0]);
        return -1;
    }

    const char *configPath = argv[1];
    int renderFPS = 24;

    if (argc == 3)
        renderFPS = std::stoi(argv[2]);

    try
    {
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

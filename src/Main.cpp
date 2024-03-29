#include "Simulation.h"
#include <iostream>

int main(void)
{
    try
    {
        Ecosim::Simulation simulation("C:/dev/projects/Ecosystem-Simulation/configs/simulations/firstSim.yaml");
        simulation.Simulate();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }

    return 0;
}

// unsigned int startTick = SDL_GetTicks();
// unsigned int numIterations = 100;
// size_t frameCount = 0;

// ++frameCount;
// if (frameCount % numIterations == 0)
// {
//     unsigned int nowTick = SDL_GetTicks();
//     std::cout << "FPS over " << numIterations << " frames: " << numIterations / ((nowTick - startTick) / 1000.0) << std::endl;
//     startTick = nowTick;
//     frameCount = 0;
// }

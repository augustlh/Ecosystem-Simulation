#include "Simulation.h"

int main(void)
{
    Ecosim::Simulation simulation("configs/simulations/firstSim.yaml");
    simulation.Simulate();
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

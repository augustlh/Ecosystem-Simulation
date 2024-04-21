#ifndef SIMULATION_H
#define SIMULATION_H

#include "Window.h"
#include <string>

typedef unsigned int uint;

namespace Ecosim
{
    class SimulationConfig
    {
    public:
        std::string name;
        uint numAgents;
        uint numFood;
        std::string enviromentConfigPath;
        uint numSeconds;
        bool storeData;

        SimulationConfig() = default;
        SimulationConfig(std::string &configPath);
        SimulationConfig(const char *configPath);
    };

    class Simulation
    {
    private:
        Window m_window;
        SimulationConfig m_config;

    public:
        // Simulation(std::string &configPath);
        Simulation(const char *configPath);
        void Simulate(uint fps);
    };
}

#endif /* SIMULATION_H */
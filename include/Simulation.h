#ifndef SIMULATION_H
#define SIMULATION_H

#include "Window.h"
#include <string>

namespace Ecosim
{
    class Simulation
    {
    private:
        Window m_window;
        int m_numAgents, m_numFood;
        std::string m_enviromentConfig;

    public:
        Simulation(const char *configPath);
        void Simulate();
    };
}

#endif /* SIMULATION_H */
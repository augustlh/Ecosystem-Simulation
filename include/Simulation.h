#ifndef SIMULATION_H
#define SIMULATION_H

#include "Window.h"
#include <string>

typedef unsigned int uint;

namespace Ecosim
{
    /// @brief A container for a `simulations/config.yaml` config file
    class ECOSIM_API SimulationConfig
    {
    public:
        /// @brief The name of a simulation
        std::string name;

        /// @brief The inital number of agents in a simulation
        uint numAgents;

        /// @brief The constant number of food-objects in a simulation
        uint numFood;

        /// @brief A path to the enviroment config
        std::string enviromentConfigPath;

        /// @brief The total number of seconds to simulate
        uint numSeconds;

        /// @brief Whether or not to store data
        bool storeData;

        /// @brief Create a blank config
        SimulationConfig() = default;

        /// @brief Create a config from a path
        /// @param configPath A valid path
        SimulationConfig(std::string &configPath);

        /// @brief Create a config from a path
        /// @param configPath A valid path
        SimulationConfig(const char *configPath);
    };

    /// @brief An instance of a simulation
    class ECOSIM_API Simulation
    {
    private:
        /// @brief A window to display the simulation
        Window m_window;

        /// @brief A simulation config with all of the user-provided settings
        SimulationConfig m_config;

    public:
        /// @brief Create a new simulation from a config file
        /// @param configPath The path to the config file
        Simulation(const char *configPath);

        /// @brief Start the simulation
        /// @param fps The fps to limit how often the simulation is rendered (or if `fps=0` no limit)
        void Simulate(uint fps);
    };
}

#endif /* SIMULATION_H */
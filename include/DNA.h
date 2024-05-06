#include <iostream>
#include <string>
#include <random>

namespace Ecosim
{
    class DNA
    {
    private:
        /// @brief The health of the agent.
        double health;

        /// @brief The energy of the agent.
        double energy;

        /// @brief The search radius of the agent.
        double searchRadius;

        /// @brief The speed of the agent.
        double speed;

        /// @brief The strength of the agent.
        double strength;

        /// @Brief Helper function to get a random value between min and max.
        double random(double min, double max) const
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<double> dis(min, max);
            return dis(gen);
        }

        /// @brief Initialize the DNA of the agent.
        void Initialize()
        {
            health = 100;
            energy = 100;

            speed = random(1.0, 5.0);
            searchRadius = random(40, 100);
            strength = random(1.0, 10.0);
        }

    public:
        DNA() { Initialize(); }
        ~DNA() = default;

        /// @brief Get the health of the agent.
        double getHealth() const { return health; }

        /// @brief Get the energy of the agent.
        double getEnergy() const { return energy; }

        /// @brief Get the search radius of the agent.
        double getSearchRadius() const { return searchRadius; }

        /// @brief Get the speed of the agent.
        double getSpeed() const { return speed; }

        /// @brief Get the strength of the agent.
        double getStrength() const { return strength; }

        /// @brief Set the health of the agent.
        void setHealth(double health) { this->health = health; }

        /// @brief Set the energy of the agent.
        void setEnergy(double energy) { this->energy = energy; }

        /// @brief Set the search radius of the agent.
        void setSearchRadius(double searchRadius) { this->searchRadius = searchRadius; }

        /// @brief Set the speed of the agent.
        void setSpeed(double speed) { this->speed = speed; }

        /// @brief Set the strength of the agent.
        void setStrength(double strength) { this->strength = strength; }

        /// @brief Mutate the DNA of the agent by adding a random value to each property.
        /// @return The mutated DNA.
        DNA Mutate()
        {
            DNA newDna;
            newDna.speed = speed + random(-0.5, 0.5);
            newDna.searchRadius = searchRadius + random(-5, 5);
            newDna.strength = strength + random(-0.5, 0.5);
            return newDna;
        }
    };
}
#include <iostream>
#include <string>
#include <random>

namespace Ecosim
{

    class DNA
    {
    private:
        /// @brief Age of the agent.
        float age;

        /// @brief maximum age of the agent.
        float maxAge;

        /// @brief The energy of the agent.
        double energy;

        /// @brief Energy depletion rate of the agent.
        double energyDepletionRate;

        /// @brief The energy gain rate of the agent.
        double metabolism;

        /// @brief The search radius of the agent.
        double searchRadius;

        /// @brief The speed of the agent.
        double speed;

        /// @brief The strength of the agent.
        double strength;

        /// @brief The fear/flee weight of the agent.
        double fearWeight; // 0-1

        /// @brief The hunger weight of the agent. (Fat? Does it value being full or does it not care?)
        double hungerWeight; // 0-1

        ///@brief Helper function to get a random value between min and max.
        ///@param min The minimum value.
        ///@param max The maximum value.
        ///@return A random value between min and max.
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
            energy = 100;

            speed = random(2.5, 4);

            searchRadius = random(40, 100);
            strength = random(1.0, 10.0);

            fearWeight = random(0, 1);

            hungerWeight = random(0.6, 1);

            energyDepletionRate = random(speed * 0.5, speed);
            metabolism = random(0.1, 1);

            age = 0;
            maxAge = random(300, 600);
        }

    public:
        DNA() { Initialize(); }
        ~DNA() = default;

        /// @brief Get the energy of the agent.
        /// @return The energy of the agent.
        double getEnergy() const { return energy; }

        /// @brief Get the search radius of the agent.
        /// @return The search radius of the agent.
        double getSearchRadius() const { return searchRadius; }

        /// @brief Get the speed of the agent.
        /// @return The speed of the agent.
        double getSpeed() const { return speed; }

        /// @brief Get the strength of the agent.
        /// @return The strength of the agent.
        double getStrength() const { return strength; }

        /// @brief Get the fear/flee weight of the agent.
        /// @return The fear/flee weight of the agent.
        double getFearWeight() const { return fearWeight; }

        /// @brief Get the hunger weight of the agent.
        /// @return The hunger weight of the agent.
        double getHungerWeight() const { return hungerWeight; }

        /// @brief Get the energy depletion rate of the agent.
        /// @return The energy depletion rate of the agent.
        double getEnergyDepletionRate() const { return energyDepletionRate; }

        /// @brief Get the metabolism of the agent.
        /// @return The metabolism of the agent.
        double getMetabolism() const { return metabolism; }

        /// @brief Set the energy of the agent.
        /// @param energy The new energy of the agent.
        void setEnergy(double energy) { this->energy = energy; }

        /// @brief Set the search radius of the agent.
        /// @param searchRadius The new search radius of the agent.
        void setSearchRadius(double searchRadius) { this->searchRadius = searchRadius; }

        /// @brief Set the speed of the agent.
        /// @param speed The new speed of the agent.
        void setSpeed(double speed) { this->speed = speed; }

        /// @brief Set the strength of the agent.
        /// @param strength The new strength of the agent.
        void setStrength(double strength) { this->strength = strength; }

        /// @brief Set the fear/flee weight of the agent.
        /// @param fearWeight The new fear/flee weight of the agent.
        void setFearWeight(double fearWeight) { this->fearWeight = fearWeight; }

        /// @brief Set the hunger weight of the agent.
        /// @param hungerWeight The new hunger weight of the agent.
        void setHungerWeight(double hungerWeight) { this->hungerWeight = hungerWeight; }

        /// @brief Set the energy depletion rate of the agent.
        /// @param energyDepletionRate The new energy depletion rate of the agent.
        void setEnergyDepletionRate(double energyDepletionRate) { this->energyDepletionRate = energyDepletionRate; }

        /// @brief Set the metabolism of the agent.
        /// @param metabolism The new metabolism of the agent.
        void setMetabolism(double metabolism) { this->metabolism = metabolism; }

        /// @brief Get the age of the agent.
        /// @return The age of the agent.
        float getAge() const { return age; }

        /// @brief Set the age of the agent.
        /// @param age The new age of the agent.
        void setAge(float age) { this->age = age; }

        /// @brief Get the maximum age of the agent.
        /// @return The maximum age of the agent.
        float getMaxAge() const { return maxAge; }

        /// @brief Returns a Mutation of the DNA. This is used when an agent reproduces.
        /// @return A mutated DNA.
        DNA Mutate()
        {
            DNA mutatedDNA = *this;

            mutatedDNA.setEnergy(50);
            mutatedDNA.setSearchRadius(mutatedDNA.getSearchRadius() + random(-5, 5));
            mutatedDNA.setSpeed(mutatedDNA.getSpeed() + random(-0.5, 0.5));
            mutatedDNA.setStrength(mutatedDNA.getStrength() + random(-0.5, 0.5));
            mutatedDNA.setFearWeight(mutatedDNA.getFearWeight() + random(-0.1, 0.1));
            mutatedDNA.setHungerWeight(mutatedDNA.getHungerWeight() + random(-0.1, 0.1));
            mutatedDNA.setEnergyDepletionRate(mutatedDNA.getEnergyDepletionRate() + random(-0.8, 0.8));
            mutatedDNA.setMetabolism(mutatedDNA.getMetabolism() + random(-0.25, 0.25));

            return mutatedDNA;
        }
    };
}
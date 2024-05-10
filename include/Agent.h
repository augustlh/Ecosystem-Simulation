#ifndef AGENT_H
#define AGENT_H

#include "Vector2.h"
#include "Renderer.h"
#include "Interfaces.h"
#include "DNA.h"

namespace Ecosim
{

    struct Observation
    {
        std::shared_ptr<Collidable> closestEatable;
        std::shared_ptr<Collidable> closestEnemy;
    };

    class Agent : public Renderable, public Simulatable, public Collidable
    {
    private:
        Vector2<float> m_Pos;
        int m_Radius;
        Color m_Color;
        DNA m_Dna;

        std::vector<std::shared_ptr<Agent>> m_Family = {};

    public:
        Agent();
        ~Agent() = default;

        /// @brief Bool representing if the agent is dead
        bool isDead = false;

        /// @brief Bool representing if the agent wants to reproduce
        bool wantsToReproduce = false;

        /// @brief Draws the agent on the screen
        void Draw() override;

        /// @brief Updates the agent one step in the simulation
        void Step(double deltaTime) override;

        // Getters and setters
        /// @brief Get the energy of the agent
        float getEnergy() override { return m_Dna.getEnergy(); }

        /// @brief Get the color of the agent
        Color GetColor() { return m_Color; }

        /// @brief Get the DNA of the agent
        DNA GetDna() { return m_Dna; }

        /// @brief Set the position of the agent
        void SetPosition(Vector2<float> pos) { m_Pos = pos; }

        /// @brief Set the color of the agent
        void SetColor(Color color) { m_Color = color; }

        /// @brief Set the DNA of the agent
        void SetDna(DNA dna) { m_Dna = dna; }

        // Collidable interface
        Vector2<float> getPosition() override;
        bool Collides(std::shared_ptr<Collidable> &other) override;
        void handleCollision(std::shared_ptr<Collidable> &other) override;
        void resolveAgentFoodCollision(std::shared_ptr<Collidable> &food);
        void resolveAgentAgentCollision(std::shared_ptr<Collidable> &agent);
        CollidableType getType() { return AGENT; }
        void OnEaten() override { isDead = true; }
        int getRadius() override { return m_Radius; }

        /// Agent specific functions
        /// @brief Updates the agent based on the action received
        void OnActionReceived(Vector2<float> &action, double deltaTime);

        /// @brief Requests an action from the agent
        Vector2<float> RequestAction(Observation &observation);

        /// @brief Updates the agent based on the time passed
        void UpdateAgent(double deltaTime);

        /// @brief Collects observations from the agent to be used in the RequestAction method
        Observation CollectObservations();

        /// @brief Adds a family member to the agent
        void AddFamilyMember(std::shared_ptr<Agent> agent) { m_Family.push_back(agent); }

        /// @brief Eats another collidable object
        void Eat(std::shared_ptr<Collidable> &other, CollidableType type = FOOD);

        /// @brief Checks if the agent can eat another collidable object
        bool IsEatable(std::shared_ptr<Collidable> &other);

        /// @brief Mutates the DNA of the agent
        /// @return A DNA object with the mutated values
        DNA MutateDNA() { return m_Dna.Mutate(); }

        /// @brief Method that updates the state of the agent to reflect consequences of reproduction
        void Reproduce();
    };

}

#endif /* AGENT_H */
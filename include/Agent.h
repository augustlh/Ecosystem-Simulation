#ifndef AGENT_H
#define AGENT_H

#include "Vector2.h"
#include "Renderer.h"
#include "Interfaces.h"
#include "DNA.h"

namespace Ecosim
{

    /// @brief A struct representing an observation of an agent. The observation contains the closest eatable and enemy objects
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

        int m_Id = 0;

        std::vector<std::shared_ptr<Collidable>> m_Family = {};

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

        /// @brief Gets the energy of the agent
        float getEnergy() override { return m_Dna.getEnergy(); }

        /// @brief Gets the color of the agent
        Color GetColor() { return m_Color; }

        /// @brief Gets the DNA of the agent
        DNA GetDna() { return m_Dna; }

        /// @brief Sets the position of the agent
        void SetPosition(Vector2<float> pos) { m_Pos = pos; }

        /// @brief Sets the color of the agent
        void SetColor(Color color) { m_Color = color; }

        /// @brief Sets the DNA of the agent
        void SetDna(DNA dna)
        {
            m_Dna = dna;
            m_Radius = int(m_Dna.getStrength() / 2);
        }

        /// @brief Sets the ID of the agent
        void SetId(int id) { m_Id = id; }

        /// @brief Gets the ID of the agent
        int GetId() { return m_Id; }

        // Collidable interface
        Vector2<float> getPosition() override;
        bool Collides(std::shared_ptr<Collidable> &other) override;
        void handleCollision(std::shared_ptr<Collidable> &other) override;
        void resolveAgentFoodCollision(std::shared_ptr<Collidable> &food);
        void resolveAgentAgentCollision(std::shared_ptr<Collidable> &agent);
        CollidableType getType() { return AGENT; }
        void OnEaten() override { isDead = true; }
        int getRadius() override { return m_Radius; }

        /// @brief Updates the agent based on the action received
        void OnActionReceived(Vector2<float> &action, double deltaTime);

        /// @brief Requests an action from the agent
        Vector2<float> RequestAction(Observation &observation);

        /// @brief Updates the agent based on the time passed
        void UpdateAgent(double deltaTime);

        /// @brief Collects observations from the agent to be used in the RequestAction method
        Observation CollectObservations();

        /// @brief Adds a family member to the agent
        void AddFamilyMember(std::shared_ptr<Collidable> agent) { m_Family.push_back(agent); }

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
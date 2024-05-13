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

    /// @brief A class representing an agent in the simulation
    class Agent : public Renderable, public Simulatable, public Collidable
    {
    private:
        /// @brief The position of the agent
        Vector2<float> m_Pos;

        /// @brief The radius of the agent
        int m_Radius;

        /// @brief The color of the agent
        Color m_Color;

        /// @brief The DNA of the agent
        DNA m_Dna;

        int m_Id = 0;

    public:
        /// @brief Default constructor
        Agent();

        /// @brief Constructor that takes a position, color and DNA
        ~Agent() = default;

        /// @brief Bool representing if the agent is dead
        bool isDead = false;

        /// @brief Bool representing if the agent wants to reproduce
        bool wantsToReproduce = false;

        /// @brief Draws the agent on the screen
        void Draw() override;

        /// @brief Updates the agent one step in the simulation
        /// @param deltaTime The time in seconds since the last frame
        void Step(double deltaTime) override;

        /// @brief Gets the energy of the agent
        /// @return The energy of the agent as a float
        float getEnergy() override { return m_Dna.getEnergy(); }

        /// @brief Gets the color of the agent
        /// @return The color of the agent
        Color GetColor() { return m_Color; }

        /// @brief Gets the DNA of the agent
        /// @return The DNA of the agent
        DNA GetDna() { return m_Dna; }

        /// @brief Sets the position of the agent
        /// @param pos The new position of the agent
        void SetPosition(Vector2<float> pos) { m_Pos = pos; }

        /// @brief Sets the color of the agent
        /// @param color The new color of the agent
        void SetColor(Color color) { m_Color = color; }

        /// @brief Sets the DNA of the agent
        /// @param dna The new DNA of the agent
        void SetDna(DNA dna)
        {
            m_Dna = dna;
            m_Radius = int(m_Dna.getStrength() / 2);
        }

        /// @brief Sets the ID of the agent
        /// @param id The new ID of the agent
        void SetId(int id) { m_Id = id; }

        /// @brief Gets the ID of the agent
        /// @return The ID of the agent
        int GetId() { return m_Id; }

        // Collidable interface
        /// @brief Gets the position of the agent
        /// @return The position of the agent
        Vector2<float> getPosition() override;

        /// @brief Checks if the agent collides with another collidable object
        /// @param other The other collidable object
        /// @return A boolean representing if the agent collides with the other collidable object
        bool Collides(std::shared_ptr<Collidable> &other) override;

        /// @brief Handles the collision with another collidable object
        /// @param other The other collidable object
        void handleCollision(std::shared_ptr<Collidable> &other) override;

        /// @brief Resolves the collision between the agent and a food object
        /// @param food The collidable object of CollidableType food
        void resolveAgentFoodCollision(std::shared_ptr<Collidable> &food);

        /// @brief Resolves the collision between the agent and another agent object
        /// @param agent The collidable object of CollidableType agent
        void resolveAgentAgentCollision(std::shared_ptr<Collidable> &agent);

        /// @brief Gets the type of the collidable object
        /// @return The type of the collidable object
        CollidableType getType() { return AGENT; }

        /// @brief Handles the agent being eaten
        void OnEaten() override { isDead = true; }

        /// @brief Gets the radius of the agent
        /// @return The radius of the agent
        int getRadius() override { return m_Radius; }

        /// @brief Updates the agent based on the action received
        /// @param action The action received
        /// @param deltaTime The time in seconds since the last frame
        void OnActionReceived(Vector2<float> &action, double deltaTime);

        /// @brief Requests an action from the agent
        /// @param observation The observation of the agent
        /// @return The action the agent wants to take
        Vector2<float> RequestAction(Observation &observation);

        /// @brief Updates the agent based on the time passed
        /// @param deltaTime The time in seconds since the last frame
        void UpdateAgent(double deltaTime);

        /// @brief Collects observations from the agent to be used in the RequestAction method
        Observation CollectObservations();

        /// @brief Eats another collidable object
        /// @param other The collidable object to eat
        /// @param type The type of the collidable object
        void Eat(std::shared_ptr<Collidable> &other, CollidableType type = FOOD);

        /// @brief Checks if the agent can eat another collidable object
        /// @param other The collidable object to check if the agent can eat
        /// @return A boolean representing if the agent can eat the other collidable object
        bool IsEatable(std::shared_ptr<Collidable> &other);

        /// @brief Mutates the DNA of the agent
        /// @return A DNA object with the mutated values
        DNA MutateDNA() { return m_Dna.Mutate(); }

        /// @brief Method that updates the state of the agent to reflect consequences of reproduction
        void Reproduce();
    };

}

#endif /* AGENT_H */
#ifndef FOOD_H
#define FOOD_H

#include "Interfaces.h"

namespace Ecosim
{
    /// @brief A class representing food in the simulation
    class Food : public Collidable, public Renderable
    {
    private:
        /// @brief The position of the food
        Vector2<float> m_Pos;
        int m_Radius = 2;

        /// @brief The energy of the food
        float m_Energy;

    public:
        /// @brief Default constructor
        Food();

        /// @brief Default destructor
        ~Food() = default;

        /// @brief Draw the food on the screen
        void Draw() override;

        /// @brief Handle collision with another collidable
        /// @param other The other collidable
        void handleCollision(std::shared_ptr<Collidable> &other) override;

        /// @brief Check if the food collides with another collidable
        /// @param other The other collidable
        /// @return True if the food collides with the other collidable, false otherwise
        bool Collides(std::shared_ptr<Collidable> &other) override;

        /// @brief Get the position of the food
        /// @return The position of the food
        Vector2<float> getPosition() override { return m_Pos; }

        /// @brief Get the radius of the food
        /// @return The radius of the food
        int getRadius() override { return m_Radius; }

        /// @brief Handle the food being eaten
        void OnEaten() override;

        /// @brief Respawn the food at a random location
        void Respawn();

        /// @brief Get the energy of the food
        /// @return The energy of the food
        float getEnergy() override { return m_Energy; }

        /// @brief Get the id of the food
        /// @return The id of the food
        int GetId() override { return -1; }

        /// @brief Get the type of the collidable
        /// @return The type of the collidable
        CollidableType getType() override { return FOOD; }
    };
}
#endif /* FOOD_H */
#ifndef FOOD_H
#define FOOD_H

#include "Interfaces.h"

namespace Ecosim
{
    class Food : public Collidable, public Renderable
    {
    private:
        /// @brief The position of the food
        Vector2<float> m_Pos;

        /// @brief The energy of the food
        float m_Energy;

    public:
        Food();
        ~Food() = default;

        /// @brief Draw the food on the screen
        void Draw() override;

        /// @brief Handle collision with another collidable
        /// @param other The other collidable
        void handleCollision(std::shared_ptr<Collidable> other) override;

        /// @brief Check if the food collides with another collidable
        /// @param other The other collidable
        /// @return True if the food collides with the other collidable, false otherwise
        bool Collides(std::shared_ptr<Collidable> other) override;

        /// @brief Get the position of the food
        /// @return The position of the food
        Vector2<float> getPosition() override { return m_Pos; }

        /// @brief Respawn the food at a random location
        void Respawn();

        /// @brief Get the energy of the food
        /// @return The energy of the food
        float getEnergy() { return m_Energy; }
    };
}

#endif /* FOOD_H */
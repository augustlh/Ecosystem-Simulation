#ifndef FOOD_H
#define FOOD_H

#include "Interfaces.h"

namespace Ecosim
{
    class Food : public Collidable, public Renderable
    {
    private:
        Vector2<float> m_Pos;
        float m_Energy;

    public:
        Food();
        ~Food() = default;

        void Draw() override;

        void handleCollision(std::shared_ptr<Collidable> other) override;
        bool Collides(std::shared_ptr<Collidable> other) override;
        Vector2<float> getPosition() override { return m_Pos; }

        void Respawn();
        float getEnergy() { return m_Energy; }
    };
}

#endif /* FOOD_H */
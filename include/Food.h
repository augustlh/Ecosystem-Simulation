#ifndef FOOD_H
#define FOOD_H

#include "Vector2.h"
#include "Interfaces.h"

namespace Ecosim
{
    class Food : public Renderable, public Collidable, public Eatable
    {
    private:
        Vector2<float> m_Pos;
        float m_Energy;

    public:
        Food(Vector2<float> pos);
        ~Food();

        void Draw() override;
        bool Collides(Collidable &other) override;

        Vector2<float> getPos() override { return m_Pos; }
        float getEnergy() { return m_Energy; }

        float OnEaten(Eatable &other) override;

        void ResetFood();
    };

}

#endif
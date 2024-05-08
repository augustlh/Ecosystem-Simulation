#ifndef AGENT_H
#define AGENT_H

#include "Vector2.h"
#include "Renderer.h"
#include "Interfaces.h"
#include "DNA.h"

namespace Ecosim
{
    class Agent : public Renderable, public Simulatable, public Collidable
    {
    private:
        Vector2<float> m_Pos;
        int m_Radius;
        Color m_Color;
        DNA m_Dna;

    public:
        Agent();
        ~Agent() = default;

        bool isDead = false;

        void Draw() override;
        void Step(double deltaTime) override;

        float getEnergy() override { return m_Dna.getEnergy(); }

        Vector2<float> getPosition() override;
        bool Collides(std::shared_ptr<Collidable> &other) override;
        void handleCollision(std::shared_ptr<Collidable> &other) override;
        void resolveAgentFoodCollision(std::shared_ptr<Collidable> &food);
        void resolveAgentAgentCollision(std::shared_ptr<Collidable> &agent);
        CollidableType getType() { return AGENT; }

        void Eat(std::shared_ptr<Collidable> &other, CollidableType type = FOOD);
        void Kill() { isDead = true; }

        void Move(Vector2<float> direction, double deltaTime);
        void SetColor(Color color) { m_Color = color; }
    };

}

#endif /* AGENT_H */
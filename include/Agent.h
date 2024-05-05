#ifndef AGENT_H
#define AGENT_H

#include "Vector2.h"
#include "Renderer.h"
#include "Interfaces.h"

namespace Ecosim
{
    class AgentDNA
    {
    public:
        float searchRadius;
        float health;
        float energy;
        float speed;

        float energyDecay;
        float healthDecay;

    public:
        AgentDNA();
        ~AgentDNA() = default;

        void Initialize();
        void Mutate();
    };

    class Agent : public Renderable, public Simulatable, public Collidable
    {
    private:
        Vector2<float> m_Pos;
        int m_Radius;
        Color m_Color;
        AgentDNA m_Dna;

    public:
        Agent();
        ~Agent() = default;

        bool isDead = false;

        void Draw() override;
        void Step(double deltaTime) override;

        void handleCollision(std::shared_ptr<Collidable> other) override;
        bool Collides(std::shared_ptr<Collidable> other) override;
        Vector2<float> getPosition() override;

        void SetColor(Color color) { m_Color = color; }
    };

}

#endif /* AGENT_H */
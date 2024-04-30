#ifndef AGENT_H
#define AGENT_H

#include "Vector2.h"
#include "Interfaces.h"
#include <vector>
#include <memory>

namespace Ecosim
{
    class Agent : public Simulatable, public Renderable, public Collidable
    {
    private:
        Vector2<float> m_Pos;
        AgentBrain m_Brain;

    public:
        Agent(Vector2<float> pos);
        ~Agent();

        void Draw() override;
        void Step() override;
        bool Collides(Collidable &other) override;
        Vector2<float> getPos() override { return m_Pos; }
    };

    class AgentBrain
    {
    private:
        std::vector<float> m_DNA;
        Agent *m_Agent;

    public:
        AgentBrain();
        ~AgentBrain();

        void Mutate();
        void Crossover(AgentBrain &other);

        void RequestAction();
    }

    /*
    Tanker omkring implementering af DNA:





    */
}

#endif /* AGENT_H */
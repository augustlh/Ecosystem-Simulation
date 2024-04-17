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

    public:
        Agent(Vector2<float> pos);
        ~Agent();

        void Draw() override;
        void Step() override;
        bool Collides(Collidable &other) override;
        Vector2<float> getPos() override { return m_Pos; }
    };

}

#endif /* AGENT_H */
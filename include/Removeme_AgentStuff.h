#ifndef AGENTSTUFF_H
#define AGENTSTUFF_H

#include "Interfaces.h"
#include "Vector2.h"
#include "Renderer.h"

namespace Ecosim::Temp
{
    class Agent : public Renderable, public Simulatable
    {
    private:
        Vector2<float> m_pos;
        Color m_col;

    public:
        Agent()
        {
            m_pos = Vector2<float>(rand() % 800, rand() % 800);
            m_col = Color(rand() % 255, rand() % 255, rand() % 255);
        }

        void Draw() override
        {
            Renderer::Circle(m_pos.Convert<int>(), 3, m_col);
        }

        void Step(double deltaTime) override
        {
            float dx = static_cast<double>(rand() % 11 - 5) * deltaTime;
            float dy = static_cast<double>(rand() % 11 - 5) * deltaTime;
            m_pos += Vector2<float>(dx, dy);
        }
    };

    class Food : public Renderable
    {
    public:
        Vector2<int> pos;

        Food()
        {
            pos = Vector2<int>(rand() % 800, rand() % 800);
        }

        void Draw() override
        {
            Renderer::Circle(pos, 1, Color(160, 60, 20));
        }
    };

} // namespace Ecosim::Temp

#endif /* AGENTSTUFF_H */
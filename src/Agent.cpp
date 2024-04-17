#include "Renderer.h"
#include "Agent.h"

namespace Ecosim
{
    Agent::Agent(Vector2<float> pos)
        : m_Pos(pos) {}

    Agent::~Agent() {}

    void Agent::Draw()
    {
        Renderer::Circle(m_Pos.x, m_Pos.y, 3, Color(255, 255, 255));
    }

    void Agent::Step()
    {
        // random walk
        m_Pos.x += ((std::rand() % 11) - 5) * 0.1;
        m_Pos.y += ((std::rand() % 11) - 5) * 0.1;
    }

    bool Agent::Collides(Collidable &other)
    {
        Vector2<float> otherPos = other.getPos();
        return std::sqrt(std::pow(m_Pos.x - otherPos.x, 2) + std::pow(m_Pos.y - otherPos.y, 2)) < 10;
    }
}
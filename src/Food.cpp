#include "Renderer.h"
#include "Food.h"

namespace Ecosim
{
    Food::Food()
    {
        m_Pos = Vector2<float>(rand() % 800, rand() % 800);
        m_Energy = rand() % 10;
    }

    void Food::Draw()
    {
        Renderer::Circle(m_Pos.Convert<int>(), 2, Color(0, 255, 20));
    }

    void Food::handleCollision(std::shared_ptr<Collidable> other) {}

    bool Food::Collides(std::shared_ptr<Collidable> other)
    {
        return false;
    }

    void Food::Respawn()
    {
        m_Pos = Vector2<float>(rand() % 800, rand() % 800);
    }
}
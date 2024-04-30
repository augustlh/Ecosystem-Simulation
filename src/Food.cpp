#include "Food.h"
#include "Renderer.h"

#include <random>

namespace Ecosim
{
    Food::Food(Vector2<float> pos) : m_Pos(pos)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.1f, 1.0f);
        m_Energy = dis(gen);
    }

    Food::~Food()
    {
    }

    void Food::Draw()
    {
        Renderer::Circle(m_Pos.Convert<int>(), 2, Color(255, 0, 255));
    }

    void Food::ResetFood()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 800);
        m_Pos = Vector2<float>(dis(gen), dis(gen));
    }

    bool Food::Collides(Collidable &other)
    {
        Vector2<float> otherPos = other.getPos();
        return std::sqrt(std::pow(m_Pos.x - otherPos.x, 2) + std::pow(m_Pos.y - otherPos.y, 2)) < 10;
    }

    float Food::OnEaten(Eatable &other)
    {
        ResetFood();
        return m_Energy;
    }
}
#include "Agent.h"
#include "CollisionHandler.h"
#include <random>

namespace Ecosim
{
    Agent::Agent()
    {
        m_Pos = Vector2<float>(rand() % 800, rand() % 800);
        m_Color = Color(255, 255, 255);
        m_Radius = 3;
        m_Dna = AgentDNA();
    }

    void Agent::Draw()
    {
        Renderer::Circle(m_Pos.Convert<int>(), 3, m_Color);
    }

    void Agent::Step(double deltaTime)
    {
        // Collect observations from CollisionHandler
        std::vector<std::shared_ptr<Collidable>> collidables;
        CollisionHandler::Query(m_Pos, 400, collidables);

        // Decide on action based on observations
        Vector2<float> direction = Vector2<float>(400, 400) - m_Pos;

        // move towards 00
        // m_Pos += direction / direction.Magnitude() * m_Dna.speed * 5 * deltaTime;
    }

    void Agent::handleCollision(std::shared_ptr<Collidable> other)
    {
        m_Color = Color(255, 0, 0);
        //hvis mad, æd
        //mad, skal dø
        //Food.Respawn();
        //hvi agent, tjek om du kan æde eller om du bliver ædt, act accordingly
        //bool isDead;
    }

    bool Agent::Collides(std::shared_ptr<Collidable> other)
    {
        Vector2<float> otherPos = other->getPosition();
        float distance = sqrt((m_Pos.x - otherPos.x, 2) - pow(m_Pos.y - otherPos.y, 2));
        return distance < 2 * m_Radius;
    }

    Vector2<float> Agent::getPosition()
    {
        return m_Pos;
    }

    // ---- AgentDNA ----

    AgentDNA::AgentDNA()
    {
        Initialize();
    }

    void AgentDNA::Initialize()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

        searchRadius = 10 + dist(gen) * 2;
        health = 100 + dist(gen) * 2;
        energy = 100 + dist(gen) * 2;
        speed = 5 + dist(gen) * 2;

        energyDecay = 0.1 + dist(gen) * 0.2;
        healthDecay = 0.1 + dist(gen) * 0.2;
    }

    void AgentDNA::Mutate()
    {
        searchRadius += static_cast<float>(rand() % 3 - 1);
        health += static_cast<float>(rand() % 3 - 1);
        energy += static_cast<float>(rand() % 3 - 1);
        speed += static_cast<float>(rand() % 3 - 1);

        energyDecay += static_cast<float>(rand() % 3 - 1);
        healthDecay += static_cast<float>(rand() % 3 - 1);
    }

}
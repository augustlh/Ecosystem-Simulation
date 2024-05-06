#include "Agent.h"
#include "CollisionHandler.h"
#include <random>

#include "Food.h"

namespace Ecosim
{
    Agent::Agent()
    {
        m_Pos = Vector2<float>(rand() % 800, rand() % 800);
        m_Color = Color(255, 255, 255);
        m_Radius = 3;
        m_Dna = DNA();
    }

    /// @brief Draw agent on screen
    void Agent::Draw()
    {
        Renderer::Circle(m_Pos.Convert<int>(), 3, m_Color);
    }

    /// @brief Ask agent to take a step in the simulation
    /// @param deltaTime
    void Agent::Step(double deltaTime)
    {
        // Collect observations from environment by querying the collision handler
        std::vector<std::shared_ptr<Collidable>> collidables;
        CollisionHandler::Query(m_Pos, m_Dna.getSearchRadius(), collidables);

        Vector2<float> closestFood = Vector2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        for (auto &collidable : collidables)
        {
            if (std::dynamic_pointer_cast<Food>(collidable))
            {
                closestFood = collidable->getPosition();
                break;
            }
        }

        Vector2<float> foodDirection = Vector2<float>(0, 0);

        if (closestFood.x != std::numeric_limits<float>::max())
        {
            foodDirection = closestFood - m_Pos;
            foodDirection.Normalize();
        }

        Move(foodDirection, deltaTime);
    }

    /// @brief Move agent in a direction, direction needs to be normalized
    void Agent::Move(Vector2<float> direction, double deltaTime)
    {
        m_Pos += direction * m_Dna.getSpeed() * 10 * deltaTime;
    }

    void Agent::handleCollision(std::shared_ptr<Collidable> other)
    {

        if (std::dynamic_pointer_cast<Food>(other))
        {
            float foodEnergy = std::dynamic_pointer_cast<Food>(other)->getEnergy();
            m_Dna.setEnergy(m_Dna.getEnergy() + foodEnergy * 0.65);
            std::dynamic_pointer_cast<Food>(other)->Respawn();
        }

        if (std::dynamic_pointer_cast<Agent>(other))
        {
        }

        // m_Color = Color(255, 0, 0);
        //  hvis mad, æd
        //  mad, skal dø
        //  Food.Respawn();
        //  hvi agent, tjek om du kan æde eller om du bliver ædt, act accordingly
        //  bool isDead;
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

    // // ---- AgentDNA ----

    // AgentDNA::AgentDNA()
    // {
    //     Initialize();
    // }

    // void AgentDNA::Initialize()
    // {
    //     std::random_device rd;
    //     std::mt19937 gen(rd());
    //     std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    //     searchRadius = 40 + dist(gen) * 40;
    //     health = 100;
    //     energy = 100;
    //     speed = 8;

    //     predator = dist(gen);
    //     prey = 1 - predator;

    //     energyDecay = 0.05;
    //     healthDecay = 0.05;
    // }

    // void AgentDNA::Mutate()
    // {
    //     searchRadius += static_cast<float>(rand() % 3 - 1);
    //     health += static_cast<float>(rand() % 3 - 1);
    //     energy += static_cast<float>(rand() % 3 - 1);
    //     speed += static_cast<float>(rand() % 3 - 1);

    //     energyDecay += static_cast<float>(rand() % 3 - 1);
    //     healthDecay += static_cast<float>(rand() % 3 - 1);
    // }

}
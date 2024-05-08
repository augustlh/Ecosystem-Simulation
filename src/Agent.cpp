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

        if (m_Dna.isPredator())
        {
            m_Color = Color(255, 0, 0);
        }
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
            if (collidable->getType() == FOOD)
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

    void Agent::handleCollision(std::shared_ptr<Collidable> &other)
    {
        if (other->getType() == FOOD)
        {
            resolveAgentFoodCollision(other);
            return;
        }

        if (other->getType() == AGENT)
        {
            resolveAgentAgentCollision(other);
            return;
        }
    }

    void Agent::resolveAgentFoodCollision(std::shared_ptr<Collidable> &food)
    {
        Eat(food, FOOD);
    }

    void Agent::resolveAgentAgentCollision(std::shared_ptr<Collidable> &agent)
    {
        auto eatableAgent = std::dynamic_pointer_cast<Agent>(agent);
        if (eatableAgent)
        {
            if (m_Dna.isPredator() && !eatableAgent->m_Dna.isPredator())
            {
                Eat(agent, AGENT);
            }

            if (!m_Dna.isPredator() && eatableAgent->m_Dna.isPredator())
            {
                eatableAgent->Eat(agent, AGENT);
            }

            if (m_Dna.isPredator() && eatableAgent->m_Dna.isPredator())
            {
                if (m_Dna.getEnergy() * m_Dna.getStrength() > eatableAgent->m_Dna.getEnergy() * eatableAgent->m_Dna.getStrength())
                {
                    Eat(agent, AGENT);
                }
                else
                {
                    eatableAgent->Eat(agent, AGENT);
                }
            }
        }
    }

    void Agent::Eat(std::shared_ptr<Collidable> &other, CollidableType type)
    {

        std::shared_ptr<Eatable> eatable = std::dynamic_pointer_cast<Eatable>(other);
        if (eatable)
        {
            m_Dna.setEnergy(m_Dna.getEnergy() + eatable->getEnergy() * 0.65);
            if (type == AGENT)
            {
                std::dynamic_pointer_cast<Agent>(other)->Kill();
                return;
            }

            if (type == FOOD)
            {
                std::dynamic_pointer_cast<Food>(other)->Respawn();
                return;
            }
        }
    }

    bool Agent::Collides(std::shared_ptr<Collidable> &other)
    {
        return (m_Pos - other->getPosition()).Magnitude() < m_Radius + 3;
    }

    Vector2<float> Agent::getPosition()
    {
        return m_Pos;
    }
}
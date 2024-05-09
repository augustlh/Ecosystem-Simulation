#include "Agent.h"
#include "CollisionHandler.h"
#include <random>
#include <memory>

#include "Food.h"

namespace Ecosim
{
    Agent::Agent()
    {
        m_Pos = Vector2<float>(rand() % 800, rand() % 800);
        m_Radius = 3;
        m_Dna = DNA();
        m_Color = Color(rand() % 255, rand() % 255, rand() % 255);
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
        // Update age and energy
        m_Dna.setAge(m_Dna.getAge() + deltaTime);
        m_Dna.setEnergy(m_Dna.getEnergy() - m_Dna.getEnergyDepletionRate() * deltaTime);

        // Check if agent is dead
        if (m_Dna.getEnergy() <= 0 || m_Dna.getAge() >= m_Dna.getMaxAge())
        {
            if (isDead == false)
            {
                Kill();
            }
            return;
        }

        // Check if agent wants to reproduce
        if (m_Dna.getEnergy() > 100 && m_Dna.getAge() > 10)
        {
            wantsToReproduce = true;
        }

        // Wrap the agent around the screen with modulo
        m_Pos.x = fmod(m_Pos.x, 800);
        m_Pos.y = fmod(m_Pos.y, 800);

        Vector2<float> direction = Vector2<float>(0, 0);

        // Find cloest object that is neither stronger than you, nor in family.
        std::vector<std::shared_ptr<Collidable>> collidables;
        CollisionHandler::Query(m_Pos, m_Dna.getSearchRadius(), collidables);

        // Remove self from collidables
        collidables.erase(std::remove_if(collidables.begin(), collidables.end(), [this](std::shared_ptr<Collidable> a)
                                         { return a.get() == this; }),
                          collidables.end());

        // Sort collidables by distance to agent
        std::sort(collidables.begin(), collidables.end(), [this](std::shared_ptr<Collidable> a, std::shared_ptr<Collidable> b)
                  { return (a->getPosition() - m_Pos).SquareMagnitude() < (b->getPosition() - m_Pos).SquareMagnitude(); });

        // Closest eatable object and cloest object to avoid
        std::shared_ptr<Collidable> closestEatable = nullptr;
        std::shared_ptr<Collidable> closestAvoid = nullptr;

        std::vector<std::shared_ptr<Collidable>> children;

        for (auto &collidable : collidables)
        {
            // Base case
            if (closestEatable != nullptr && closestAvoid != nullptr)
            {
                break;
            }

            // Skip iteration if collidable is of its own kind
            if (std::find(children.begin(), children.end(), collidable) != children.end())
            {
                continue;
            }

            // Check if collidable can be eaten (agent or food). If it can't be eaten check if it should be avoided.
            if (closestEatable == nullptr)
            {
                if (collidable->getType() == AGENT)
                {
                    auto agent = std::dynamic_pointer_cast<Agent>(collidable);
                    if (agent->m_Dna.getStrength() * agent->getEnergy() < m_Dna.getStrength() * getEnergy())
                    {
                        closestEatable = collidable;
                        continue;
                    }
                }

                else if (collidable->getType() == FOOD)
                {
                    closestEatable = collidable;
                    continue;
                }
            }

            if (closestAvoid == nullptr)
            {
                if (collidable->getType() == AGENT)
                {
                    auto agent = std::dynamic_pointer_cast<Agent>(collidable);
                    if (agent->m_Dna.getStrength() * agent->getEnergy() > m_Dna.getStrength() * getEnergy())
                    {
                        closestAvoid = collidable;
                        continue;
                    }
                }
            }
        }

        if (closestEatable != nullptr)
        {
            direction += (closestEatable->getPosition() - m_Pos).Normalize() * m_Dna.getHungerWeight();
        }

        if (closestAvoid != nullptr)
        {
            direction += (m_Pos - closestAvoid->getPosition()).Normalize() * m_Dna.getFearWeight();
        }

        if (direction.Magnitude() == 0)
        {
            direction = Vector2<float>(rand() % 12 - 1, rand() % 12 - 1).Normalize();
        }

        Move(direction, deltaTime);
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

        if (m_Dna.getStrength() * m_Dna.getEnergy() > eatableAgent->m_Dna.getStrength() * eatableAgent->m_Dna.getEnergy())
        {
            Eat(agent, AGENT);
        }
        else
        {
            eatableAgent->m_Dna.setEnergy(eatableAgent->m_Dna.getEnergy() + m_Dna.getEnergy() * eatableAgent->m_Dna.getMetabolism());
            Kill();
        }
    }

    void Agent::Eat(std::shared_ptr<Collidable> &other, CollidableType type)
    {

        std::shared_ptr<Eatable> eatable = std::dynamic_pointer_cast<Eatable>(other);
        if (eatable)
        {
            m_Dna.setEnergy(m_Dna.getEnergy() + eatable->getEnergy() * m_Dna.getMetabolism());
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
        return (m_Pos - other->getPosition()).Magnitude() < m_Radius * 2;
    }

    Vector2<float> Agent::getPosition()
    {
        return m_Pos;
    }

}
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
        m_Dna = DNA();
        m_Radius = int(m_Dna.getStrength() / 2);
        m_Color = Color(rand() % 255, rand() % 255, rand() % 255);
    }

    void Agent::Draw()
    {
        Renderer::Circle(m_Pos.Convert<int>(), m_Radius, m_Color);
    }

    void Agent::Step(double deltaTime)
    {
        UpdateAgent(deltaTime);

        if (m_Dna.getAge() < 0.25)
        {
            return;
        }

        Observation obs = CollectObservations();
        Vector2<float> action = RequestAction(obs);
        OnActionReceived(action, deltaTime);
    }

    void Agent::UpdateAgent(double deltaTime)
    {
        // Update age and energy
        m_Dna.setAge(m_Dna.getAge() + deltaTime);
        m_Dna.setEnergy(m_Dna.getEnergy() - m_Dna.getEnergyDepletionRate() * deltaTime);

        // Check if agent is dead
        if (m_Dna.getEnergy() <= 0 || m_Dna.getAge() >= m_Dna.getMaxAge())
        {
            if (isDead == false)
            {
                std::cout << "Agent " << m_Id << " died at age " << m_Dna.getAge() << " with energy " << m_Dna.getEnergy() << std::endl;
                isDead = true;
            }
            return;
        }

        // Check if agent wants to reproduce
        if (m_Dna.getEnergy() > 125 && m_Dna.getAge() > 10)
        {
            wantsToReproduce = true;
        }
    }

    Observation Agent::CollectObservations()
    {
        // Find cloest object that is neither stronger than you, nor in family.
        std::vector<std::shared_ptr<Collidable>> collidables;
        CollisionHandler::Query(m_Pos, m_Dna.getSearchRadius(), collidables);

        // Remove self from collidables
        collidables.erase(std::remove_if(collidables.begin(), collidables.end(), [this](std::shared_ptr<Collidable> a)
                                         { return a.get() == this; }),
                          collidables.end());

        Observation obs;
        float minEatableDistance = std::numeric_limits<float>::max();
        float minAvoidDistance = std::numeric_limits<float>::max();

        // Find closest eatable and enemy by iterating through all collidables
        for (auto &collidable : collidables)
        {
            if (m_Id == collidable->GetId())
            {
                continue;
            }

            float distance = (collidable->getPosition() - m_Pos).Magnitude();

            if (IsEatable(collidable) && distance < minEatableDistance)
            {
                obs.closestEatable = collidable;
                minEatableDistance = distance;
            }

            if (!IsEatable(collidable) && distance < minAvoidDistance)
            {
                obs.closestEnemy = collidable;
                minAvoidDistance = distance;
            }
        }

        return obs;
    }

    Vector2<float> Agent::RequestAction(Observation &observation)
    {
        Vector2<float> action = Vector2<float>(0, 0);
        if (observation.closestEatable != nullptr)
        {
            action += (observation.closestEatable->getPosition() - m_Pos).Normalize() * m_Dna.getHungerWeight();
        }

        if (observation.closestEnemy != nullptr)
        {
            action += (m_Pos - observation.closestEnemy->getPosition()).Normalize() * m_Dna.getFearWeight();
        }

        if (action.Magnitude() == 0)
        {
            action = Vector2<float>(rand() % 12 - 1, rand() % 12 - 1).Normalize();
        }

        return action;
    }

    void Agent::OnActionReceived(Vector2<float> &action, double deltaTime)
    {
        m_Pos += action * m_Dna.getSpeed() * 10 * deltaTime;

        m_Pos.x = fmod(m_Pos.x, 800 - m_Radius);
        m_Pos.y = fmod(m_Pos.y, 800 - m_Radius);
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
        if (m_Id == agent->GetId())
        {
            return;
        }

        if (IsEatable(agent))
        {
            Eat(agent, AGENT);
        }
        else
        {
            auto eatableAgent = std::dynamic_pointer_cast<Agent>(agent);
            eatableAgent->m_Dna.setEnergy(eatableAgent->m_Dna.getEnergy() + m_Dna.getEnergy() * eatableAgent->m_Dna.getMetabolism());
            OnEaten();
        }
    }

    void Agent::Eat(std::shared_ptr<Collidable> &other, CollidableType type)
    {
        m_Dna.setEnergy(m_Dna.getEnergy() + other->getEnergy() * m_Dna.getMetabolism());
        other->OnEaten();
    }

    bool Agent::Collides(std::shared_ptr<Collidable> &other)
    {
        return (m_Pos - other->getPosition()).Magnitude() < m_Radius + other->getRadius();
    }

    Vector2<float> Agent::getPosition()
    {
        return m_Pos;
    }

    void Agent::Reproduce()
    {
        m_Dna.setEnergy(m_Dna.getEnergy() / 2);
        wantsToReproduce = false;
    }

    bool Agent::IsEatable(std::shared_ptr<Collidable> &other)
    {
        if (other->GetId() == m_Id)
            return false;

        const Agent *agent = dynamic_cast<Agent *>(other.get());
        if (agent != nullptr)
        {
            return m_Dna.getStrength() > agent->m_Dna.getStrength();
        }
        return true;
    }
}
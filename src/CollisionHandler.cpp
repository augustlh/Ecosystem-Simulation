#include "CollisionHandler.h"
#include "QuadTree.h"
#include <algorithm>

namespace Ecosim
{
    std::vector<std::shared_ptr<Collidable>> CollisionHandler::m_Collidables;
    QuadTree CollisionHandler::m_QuadTree(Node(Vector2<float>(400, 400), 800));

    void CollisionHandler::Rebuild()
    {
        m_QuadTree.Clear();

        for (auto &collidable : m_Collidables)
        {
            m_QuadTree.Insert(collidable);
        }
    }

    void CollisionHandler::CheckCollisions()
    {
        for (auto &collidable : m_Collidables)
        {
            std::vector<std::shared_ptr<Collidable>> found;
            m_QuadTree.Query(Node(collidable->getPosition(), 20), found);

            found.erase(std::remove(found.begin(), found.end(), collidable), found.end());

            for (auto &other : found)
            {
                if (collidable->Collides(other))
                {
                    collidable->handleCollision(other);
                }
            }
        }
    }

    void CollisionHandler::Query(Vector2<float> position, float radius, std::vector<std::shared_ptr<Collidable>> &collidables)
    {
        m_QuadTree.Query(Node(position, radius), collidables);

        std::sort(collidables.begin(), collidables.end(), [position](std::shared_ptr<Collidable> a, std::shared_ptr<Collidable> b)
                  { return (a->getPosition() - position).Magnitude() < (b->getPosition() - position).Magnitude(); });
        return;
    }

    void CollisionHandler::AddCollidable(std::shared_ptr<Collidable> collidable)
    {
        m_Collidables.push_back(collidable);
        // m_QuadTree.Insert(collidable);
    }

    void CollisionHandler::RemoveCollidable(std::shared_ptr<Collidable> collidable)
    {
        m_Collidables.erase(std::remove(m_Collidables.begin(), m_Collidables.end(), collidable), m_Collidables.end());
    }
}
#include "CollisionHandler.h"
#include "QuadTree.h"
#include <algorithm>
#include <unordered_set>

#include "Food.h"

namespace Ecosim
{
    std::vector<std::shared_ptr<Collidable>> *CollisionHandler::m_Collidables;
    QuadTree CollisionHandler::m_QuadTree(Node(Vector2<float>(400, 400), 800));

    void CollisionHandler::Rebuild()
    {
        m_QuadTree.Clear();

        for (auto &collidable : *m_Collidables)
        {
            m_QuadTree.Insert(collidable);
        }
    }

    void CollisionHandler::CheckCollisions()
    {
        // Declares an unordered set of shared pointers to collidables called checked. This set will be used to keep track of which collidables have already been checked for collisions.
        std::unordered_set<std::shared_ptr<Collidable>> checked;

        // Iterates through all collidables in the m_Collidables vector.
        for (auto &collidable : *m_Collidables)
        {
            // If the collidable is a food object, skip the current iteration. If collisions are checked for agents, food will naturally also be checked.
            if (collidable->getType() == CollidableType::FOOD)
            {
                continue;
            }

            std::vector<std::shared_ptr<Collidable>> found;               // Declares a vector of shared pointers to collidables called found. This vector will be used to store collidables that are found within a certain range of the current collidable.
            m_QuadTree.Query(Node(collidable->getPosition(), 20), found); // Queries the quadtree for collidables within a range of 20 units of the current collidable.

            found.erase(std::remove(found.begin(), found.end(), collidable), found.end()); // Removes the current collidable from the found vector, so that it is not checked for collisions with itself.

            // Iterates through all collidables found within the range of the current collidable.
            for (auto &other : found)
            {
                // If the other collidable has already been checked for collisions, skip the current iteration. This prevents duplicate collision checks but also limits total number of collisions each frame to 1 per collidable.
                if (checked.find(other) != checked.end())
                {
                    continue;
                }

                // If the current collidable collides with the other collidable, handle the collision and add both collidables to the checked set.
                if (collidable->Collides(other))
                {
                    collidable->handleCollision(other);
                    checked.insert(collidable);
                    checked.insert(other);
                }
            }
        }
    }

    void CollisionHandler::Query(Vector2<float> position, float radius, std::vector<std::shared_ptr<Collidable>> &collidables)
    {
        m_QuadTree.Query(Node(position, radius), collidables);
    }

    void CollisionHandler::SetCollidables(std::vector<std::shared_ptr<Collidable>> &collidables)
    {
        m_Collidables = &collidables;
    }
}
#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include <vector>
#include <memory>
#include "Interfaces.h"
#include "Vector2.h"
#include "QuadTree.h"

namespace Ecosim
{
    class CollisionHandler
    {
    public:
        /// @brief Rebuilds the quadtree
        static void Rebuild();

        /// @brief Checks for collisions between collidables
        static void CheckCollisions();

        /// @brief Queries the quadtree for collidables within a certain radius of a position
        static void Query(Vector2<float> position, float radius, std::vector<std::shared_ptr<Collidable>> &collidables);

        /// @brief Sets the collidables to be used in the quadtree
        static void SetCollidables(std::vector<std::shared_ptr<Collidable>> &collidables);

        /// @brief Renders the quadtree
        static void Render() { m_QuadTree.Render(); }

    private:
        /// @brief The collidables to be used in the quadtree
        static std::vector<std::shared_ptr<Collidable>> m_Collidables;

        /// @brief The quadtree used for collision detection
        static QuadTree m_QuadTree;
    };
}

#endif /* COLLISIONHANDLER_H */

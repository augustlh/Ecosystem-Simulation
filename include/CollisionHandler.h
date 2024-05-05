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
        static void Rebuild();
        static void CheckCollisions();
        static void Query(Vector2<float> position, float radius, std::vector<std::shared_ptr<Collidable>> &collidables);

        static void SetCollidables(std::vector<std::shared_ptr<Collidable>> &collidables);

        static void AddCollidable(std::shared_ptr<Collidable> collidable);
        static void RemoveCollidable(std::shared_ptr<Collidable> collidable);

        static void Render() { m_QuadTree.Render(); }

    private:
        static std::vector<std::shared_ptr<Collidable>> m_Collidables;
        static QuadTree m_QuadTree;
    };
}

#endif /* COLLISIONHANDLER_H */

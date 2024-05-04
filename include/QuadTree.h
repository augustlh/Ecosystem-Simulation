#ifndef QT_H
#define QT_H

#include <memory>
#include <vector>

#include "Vector2.h"
#include "Interfaces.h"

namespace Ecosim
{
    class Node
    {
    private:
        const Vector2<float> m_Pos;
        const int m_Width;

    public:
        Node(const Vector2<float> _pos, const int _width) : m_Pos(_pos), m_Width(_width) {}
        ~Node() = default;

        bool Contains(Vector2<float> &point);
        bool IntersectsNode(Node &other);

        void Render();

        int GetWidth() { return m_Width; }
        Vector2<float> GetPos() { return m_Pos; }
    };

    class QuadTree
    {
    private:
        const int QT_NODE_CAPACITY = 4;
        Node m_Boundary;
        std::vector<std::shared_ptr<Collidable>> m_Collidables;
        std::vector<std::unique_ptr<QuadTree>> m_Children;

    public:
        QuadTree(Node &_boundary) : m_Boundary(_boundary), m_Children(4) {}
        ~QuadTree() = default;

        bool Insert(std::shared_ptr<Collidable> collidable);
        void Subdivide();
        void Query(Node &range, std::vector<std::shared_ptr<Collidable>> &found);
        void Clear();

        void Render();
    };
}

#endif /* QT_H */
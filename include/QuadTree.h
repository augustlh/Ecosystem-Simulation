#ifndef QT_H
#define QT_H

#include "Vector2.h"
#include "Renderer.h"
#include <vector>
#include <memory>

namespace Ecosim
{

    class Node
    {
    public:
        Node(Vector2<int> _center, int _width)
            : center(_center), width(_width) {}
        ~Node();

        float width;
        Vector2<int> center;

        bool contains(Vector2<int> point);
        void Render()
        {

            Vector2<float> halfSize(width / 2, width / 2);
            Vector2<float> topLeft(center.x - halfSize.x, center.y - halfSize.y);
            Vector2<float> topRight(center.x + halfSize.x, center.y - halfSize.y);
            Vector2<float> bottomLeft(center.x - halfSize.x, center.y + halfSize.y);
            Vector2<float> bottomRight(center.x + halfSize.x, center.y + halfSize.y);

            Renderer::Line(topLeft, topRight, 1, Color(255, 255, 255));
            Renderer::Line(topRight, bottomRight, 1, Color(255, 255, 255));
            Renderer::Line(bottomRight, bottomLeft, 1, Color(255, 255, 255));
            Renderer::Line(bottomLeft, topLeft, 1, Color(255, 255, 255));
        }
    };

    class QuadTree
    {
    public:
        QuadTree(Node _boundary, unsigned int _QT_DEPTH)
            : boundary(_boundary), depth(_QT_DEPTH), quadrants(4, nullptr) {}
        ~QuadTree();

        Node boundary;
        int depth;
        std::vector<std::shared_ptr<QuadTree>> quadrants;
        std::vector<Vector2<int>> points;

        void Subdivide();
        void Insert(const Vector2<int> &point);
        void Query(Vector2<int> range, std::vector<Vector2<int>> &found);

        void Render()
        {
            boundary.Render();
            for (auto &quadrant : quadrants)
            {
                if (quadrant != nullptr)
                    quadrant->Render();
            }
        }
    };
}

#endif
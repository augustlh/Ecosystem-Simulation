#include "QuadTree.h"
#include <vector>

namespace Ecosim
{
    bool Node::contains(Vector2<int> point)
    {
        return center.x - width / 2 <= point.x && center.x + width / 2 >= point.x && center.y - width / 2 <= point.y && center.y + width / 2 >= point.y;
    }

    Node::~Node() {}

    QuadTree::~QuadTree()
    {
        delete this;
    }

    void QuadTree::Insert(const Vector2<int> &point)
    {
        if (points.size() < depth)
        {
            points.push_back(point);
            return;
        }
        else
        {
            if (quadrants[0] == nullptr)
                Subdivide();

            for (auto &quadrant : quadrants)
            {
                if (quadrant != nullptr && quadrant->boundary.contains(point))
                {
                    quadrant->Insert(point);
                    return;
                }
            }
        }
    }

    void QuadTree::Subdivide()
    {
        float x = boundary.center.x;
        float y = boundary.center.y;
        float w = boundary.width / 2;

        Node nw(Vector2<int>(x - w / 2, y - w / 2), w);
        Node ne(Vector2<int>(x + w / 2, y - w / 2), w);
        Node sw(Vector2<int>(x - w / 2, y + w / 2), w);
        Node se(Vector2<int>(x + w / 2, y + w / 2), w);

        quadrants[0] = std::make_shared<QuadTree>(nw, depth);
        quadrants[1] = std::make_shared<QuadTree>(ne, depth);
        quadrants[2] = std::make_shared<QuadTree>(sw, depth);
        quadrants[3] = std::make_shared<QuadTree>(se, depth);
    }
}
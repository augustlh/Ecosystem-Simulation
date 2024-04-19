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
        Node(const Vector2<int> _pos, int _width) : pos(_pos), width(_width) {}
        ~Node() {}

        const Vector2<int> pos;
        const int width;

        bool Contains(Vector2<float> &point)
        {
            return pos.x - width / 2 <= point.x && pos.x + width / 2 >= point.x && pos.y - width / 2 <= point.y && pos.y + width / 2 >= point.y;
        }
        bool IntersectsNode(Node &other)
        {
            return pos.x - width / 2 <= other.pos.x + other.width / 2 && pos.x + width / 2 >= other.pos.x - other.width / 2 && pos.y - width / 2 <= other.pos.y + other.width / 2 && pos.y + width / 2 >= other.pos.y - other.width / 2;
        }

        void Render()
        {
            Vector2<float> halfSize(static_cast<float>(width) / 2, static_cast<float>(width) / 2);
            Vector2<float> topLeft(pos.x - halfSize.x, pos.y - halfSize.y);
            Vector2<float> topRight(pos.x + halfSize.x, pos.y - halfSize.y);
            Vector2<float> bottomLeft(pos.x - halfSize.x, pos.y + halfSize.y);
            Vector2<float> bottomRight(pos.x + halfSize.x, pos.y + halfSize.y);

            Renderer::Line(topLeft, topRight, 1, Color(255, 255, 255));
            Renderer::Line(topRight, bottomRight, 1, Color(255, 255, 255));
            Renderer::Line(bottomRight, bottomLeft, 1, Color(255, 255, 255));
            Renderer::Line(bottomLeft, topLeft, 1, Color(255, 255, 255));
        }
    };

    template <typename T>
    class QuadTree
    {
    public:
        QuadTree(Node &_boundary, const int _MAX_ITEMS) : boundary(_boundary), MAX_ITEMS(_MAX_ITEMS), children(4)
        {
        }
        ~QuadTree() {}

        Node boundary;
        const int MAX_ITEMS;
        std::vector<std::unique_ptr<QuadTree<T>>> children;
        std::vector<T> data;

        bool Insert(const T &item)
        {
            // If the item is not within the boundary of the current node, return false
            if (!boundary.Contains(item->getPos()))
            {
                return false;
            }

            // If the current node has less than MAX_ITEMS and no children, insert the item and return true
            if (data.size() < MAX_ITEMS && children[0] == 0)
            {
                data.push_back(item);
                return true;
            }

            // Else if the current node has less than MAX_ITEMS and has children, insert the item into the children
            if (children[0] == nullptr)
            {
                Subdivide();
            }

            // Insert the item into the children
            if (children[0]->Insert(item))
                return true;
            if (children[1]->Insert(item))
                return true;
            if (children[2]->Insert(item))
                return true;
            if (children[3]->Insert(item))
                return true;

            // std::cout << "Error: Could not insert item into any of the children" << std::endl;
            // // Print the item's position for debugging
            // std::cout << "Item position: " << item->getPos().x << ", " << item->getPos().y << std::endl;
            // draw a red line to the item

            return false;
        }

        void Subdivide()
        {
            // Calculate the new width
            int w = boundary.width / 2;

            // Calculate the position of the topLeft quadrant
            Vector2<int> nw = Vector2<int>(boundary.pos.x - w / 2, boundary.pos.y - w / 2);

            // Create new child nodes (quadrants) using smart pointers
            children[0] = std::make_unique<QuadTree<T>>(Node(nw, w), MAX_ITEMS);
            children[1] = std::make_unique<QuadTree<T>>(Node(Vector2<int>(nw.x + w, nw.y), w), MAX_ITEMS);
            children[2] = std::make_unique<QuadTree<T>>(Node(Vector2<int>(nw.x, nw.y + w), w), MAX_ITEMS);
            children[3] = std::make_unique<QuadTree<T>>(Node(Vector2<int>(nw.x + w, nw.y + w), w), MAX_ITEMS);

            // Insert the data of the "father" into the children
            for (auto &item : data)
            {
                if (children[0]->Insert(item))
                    continue;
                if (children[1]->Insert(item))
                    continue;
                if (children[2]->Insert(item))
                    continue;
                if (children[3]->Insert(item))
                    continue;
            }

            // Clear the data of the "father"
            data.clear();
        }

        void Query(Node &range, std::vector<T> &found)
        {
            // If the boundary of the current node does not intersect with the range, return
            if (!boundary.IntersectsNode(range))
                return;

            // If the boundary of the current node is contained within the range, add all data to the found vector
            for (auto &item : data)
            {
                if (range.Contains(item->getPos()))
                    found.push_back(item);
            }

            // If the current node has children, query the children
            if (children[0] == nullptr)
                return;

            children[0]->Query(range, found);
            children[1]->Query(range, found);
            children[2]->Query(range, found);
            children[3]->Query(range, found);
        }

        void Render()
        {
            boundary.Render();

            for (auto &child : children)
            {
                if (child != nullptr)
                    child->Render();
            }
        }

        void Clear()
        {
            data.clear();
            for (auto &child : children)
            {
                if (child != nullptr)
                    child->Clear();
            }

            children.clear();
            children.resize(4);
        }
    };
}

#endif
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
        ~Node() {}

        float width;
        Vector2<int> center;

        bool contains(Vector2<int> point)
        {
            return center.x - width / 2 <= point.x && center.x + width / 2 >= point.x && center.y - width / 2 <= point.y && center.y + width / 2 >= point.y;
        }
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

    template <typename T>
    class QuadTree
    {
    public:
        QuadTree(Node _boundary, unsigned int _QT_DEPTH)
            : boundary(_boundary), depth(_QT_DEPTH), quadrants(4, nullptr) {}
        ~QuadTree() {}

        int depth;
        int MAX_ITEMS = 4;

        Node boundary;
        std::vector<std::shared_ptr<QuadTree<T>>> quadrants;
        std::vector<T> data;

        /**
         * @brief Subdivide the current node into 4 quadrants
         */
        void Subdivide()
        {
            // Calculates the new width
            float w = boundary.width / 2;

            // Calculates the position of the topLeft quadrant
            Vector2<int> nw = Vector2<int>(boundary.center.x - w / 2, boundary.center.y - w / 2);

            // Creates new child nodes (quadrants) using smart pointers
            quadrants[0] = std::make_shared<QuadTree<T>>(Node(nw, w), depth);
            quadrants[1] = std::make_shared<QuadTree<T>>(Node(Vector2<int>(nw.x + w, nw.y), w), depth - 1);
            quadrants[2] = std::make_shared<QuadTree<T>>(Node(Vector2<int>(nw.x, nw.y + w), w), depth - 1);
            quadrants[3] = std::make_shared<QuadTree<T>>(Node(Vector2<int>(nw.x + w, nw.y + w), w), depth - 1);
        }

        /**
         * @brief Insert an item into the quadtree
         * @param item The item to insert
         */
        void Insert(const T &item)
        {
            // If the current node has less than MAX_ITEMS or the depth is 0, insert the item and return
            if (data.size() < MAX_ITEMS || depth == 0)
            {
                data.push_back(item);
                return;
            }
            else
            {
                // If the quadrants are not initialized, subdivide the current node
                if (quadrants[0] == nullptr)
                {
                    Subdivide();
                }

                // Insert the item into the quadrant that contains the item
                for (auto &quadrant : quadrants)
                {
                    if (quadrant != nullptr && quadrant->boundary.contains(item->getPos().Convert<int>()))
                    {
                        quadrant->Insert(item);
                        return;
                    }
                }
            }
        }

        /**
         * @brief Query the quadtree for items within a given range
         * @param range The range to query
         * @param found The vector to store the found items
         */
        void Query(Vector2<int> range, std::vector<T> &found)
        {
            // Check if the range intersects with the boundary of the current node
            if (range.x + range.y < boundary.center.x - boundary.width / 2 || range.x - range.y > boundary.center.x + boundary.width / 2 || range.y + range.x < boundary.center.y - boundary.width / 2 || range.y - range.x > boundary.center.y + boundary.width / 2)
            {
                return;
            }

            // If the current node is a leaf node (a region that can't be subdivided further), add all items to the found vector
            if (quadrants[0] == nullptr)
            {
                for (const auto &item : data)
                {
                    found.push_back(item);
                }
            }
            else
            {
                // Recursively query the child nodes (quadrants) for items within the range
                for (const auto &quadrant : quadrants)
                {
                    if (quadrant != nullptr)
                    {
                        quadrant->Query(range, found);
                    }
                }
            }
        }

        /**
         * @brief Renders the quadtree
         */
        void Render()
        {
            // Render the boundary of the current node
            boundary.Render();

            // Recursively render the child nodes (quadrants)
            for (const auto &quadrant : quadrants)
            {
                if (quadrant != nullptr)
                {
                    quadrant->Render();
                }
            }
        }

        /**
         * @brief Clears the quadtree
         */
        void Clear()
        {
            data.clear();
            for (auto &quadrant : quadrants)
            {
                if (quadrant != nullptr)
                {
                    quadrant->Clear();
                }
            }
        }
    };
}

#endif
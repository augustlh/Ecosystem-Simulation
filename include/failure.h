// #ifndef QT_H
// #define QT_H

// #include "Vector2.h"
// #include "Renderer.h"
// #include <vector>
// #include <memory>

// namespace Ecosim
// {

//     class Node
//     {
//     public:
//         Node(Vector2<int> _center, int _width)
//             : center(_center), width(_width) {}
//         ~Node() {}

//         float width;
//         Vector2<int> center;

//         bool contains(Vector2<int> point)
//         {
//             return center.x - width / 2 <= point.x && center.x + width / 2 >= point.x && center.y - width / 2 <= point.y && center.y + width / 2 >= point.y;
//         }
//         void Render()
//         {

//             Vector2<float> halfSize(width / 2, width / 2);
//             Vector2<float> topLeft(center.x - halfSize.x, center.y - halfSize.y);
//             Vector2<float> topRight(center.x + halfSize.x, center.y - halfSize.y);
//             Vector2<float> bottomLeft(center.x - halfSize.x, center.y + halfSize.y);
//             Vector2<float> bottomRight(center.x + halfSize.x, center.y + halfSize.y);

//             Renderer::Line(topLeft, topRight, 1, Color(255, 255, 255));
//             Renderer::Line(topRight, bottomRight, 1, Color(255, 255, 255));
//             Renderer::Line(bottomRight, bottomLeft, 1, Color(255, 255, 255));
//             Renderer::Line(bottomLeft, topLeft, 1, Color(255, 255, 255));
//         }
//     };

//     template <typename T>
//     class QuadTree
//     {
//     public:
//         QuadTree(Node _boundary, unsigned int _QT_DEPTH)
//             : boundary(_boundary), depth(_QT_DEPTH), quadrants(4, nullptr) {}
//         ~QuadTree() {}

//         int depth;
//         int MAX_ITEMS = 2;

//         Node boundary;
//         std::vector<std::shared_ptr<QuadTree<T>>> quadrants;
//         std::vector<T> data;

//         /**
//          * @brief Subdivide the current node into 4 quadrants
//          */
//         void Subdivide()
//         {
//             // Calculates the new width
//             float w = boundary.width / 2;

//             // Calculates the position of the topLeft quadrant
//             Vector2<int> nw = Vector2<int>(boundary.center.x - w / 2, boundary.center.y - w / 2);

//             // Creates new child nodes (quadrants) using smart pointers
//             quadrants[0] = std::make_shared<QuadTree<T>>(Node(nw, w), depth);
//             quadrants[1] = std::make_shared<QuadTree<T>>(Node(Vector2<int>(nw.x + w, nw.y), w), depth - 1);
//             quadrants[2] = std::make_shared<QuadTree<T>>(Node(Vector2<int>(nw.x, nw.y + w), w), depth - 1);
//             quadrants[3] = std::make_shared<QuadTree<T>>(Node(Vector2<int>(nw.x + w, nw.y + w), w), depth - 1);
//         }

//         /**
//          * @brief Insert an item into the quadtree
//          * @param item The item to insert
//          */
//         void Insert(const T &item)
//         {
//             // If the current node has less than MAX_ITEMS or the depth is 0, insert the item and return
//             if (data.size() < MAX_ITEMS || depth == 0)
//             {
//                 data.push_back(item);
//                 return;
//             }
//             else
//             {
//                 // If the quadrants are not initialized, subdivide the current node
//                 if (quadrants[0] == nullptr)
//                 {
//                     Subdivide();
//                 }

//                 // Insert the item into the quadrant that contains the item
//                 for (auto &quadrant : quadrants)
//                 {
//                     if (quadrant != nullptr && quadrant->boundary.contains(item->getPos().Convert<int>()))
//                     {
//                         quadrant->Insert(item);
//                         return;
//                     }
//                 }
//             }
//         }

//         /**
//          * @brief Query the quadtree for items within a given range
//          * @param range The range to query
//          * @param found The vector to store the found items
//          */
//         void Query(Vector2<int> pos, int width, std::vector<T> &found)
//         {
//             // Check if the range intersects with the boundary of the current node
//             if (pos.x + width < boundary.center.x - boundary.width / 2 || pos.x - width > boundary.center.x + boundary.width / 2 || pos.y + width < boundary.center.y - boundary.width / 2 || pos.y - width > boundary.center.y + boundary.width / 2)
//             {
//                 return;
//             }

//             // If the current node is a leaf node (a region that can't be subdivided further), add all items to the found vector
//             if (quadrants[0] == nullptr)
//             {
//                 for (const auto &item : data)
//                 {
//                     found.push_back(item);
//                 }
//             }
//             else
//             {
//                 // Recursively query the child nodes (quadrants) for items within the range
//                 for (const auto &quadrant : quadrants)
//                 {
//                     if (quadrant != nullptr)
//                     {
//                         quadrant->Query(pos, width, found);
//                     }
//                 }
//             }
//         }

//         /**
//          * @brief Renders the quadtree
//          */
//         void Render()
//         {
//             // Render the boundary of the current node
//             boundary.Render();

//             // Recursively render the child nodes (quadrants)
//             for (const auto &quadrant : quadrants)
//             {
//                 if (quadrant != nullptr)
//                 {
//                     quadrant->Render();
//                 }
//             }
//         }

//         /**
//          * @brief Clears the quadtree
//          */
//         void Clear()
//         {
//             data.clear();
//             for (auto &quadrant : quadrants)
//             {
//                 if (quadrant != nullptr)
//                 {
//                     quadrant->Clear();
//                 }
//             }
//         }
//     };
// }

// #endif

#ifndef QUADTREE_H
#define QUADTREE_H

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
        ~Node();

        const Vector2<int> pos;
        const int width;

        bool Contains(Vector2<float> &point)
        {
            return pos.x - width / 2 < point.x && pos.x + width / 2 > point.x && pos.y - width / 2 < point.y && pos.y + width / 2 > point.y;
        }

        bool IntersectsNode(Node &other)
        {
            return pos.x - width / 2 < other.pos.x + other.width / 2 && pos.x + width / 2 > other.pos.x - other.width / 2 && pos.y - width / 2 < other.pos.y + other.width / 2 && pos.y + width / 2 > other.pos.y - other.width / 2;
        }

        void Render()
        {
            Vector2<float> halfSize(width / 2, width / 2);
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
        QuadTree(Node &_boundary, const int _MAX_ITEMS) : boundary(_boundary), MAX_ITEMS(_MAX_ITEMS), children(4, nullptr)
        {
            static_assert(std::is_member_function_pointer<decltype(&T::getPos)>::value, "T must have a getPos() method");
        }
        ~QuadTree() {}

        Node boundary;
        const int MAX_ITEMS;

        std::vector<T> data;
        std::vector<std::unique_ptr<QuadTree<T>>> children;

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
            for (auto &child : children)
            {
                if (child->Insert(item))
                {
                    return true;
                }
            }

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
                for (auto &child : children)
                {
                    if (child->Insert(item))
                    {
                        break;
                    }
                }
            }
        }

        void Query(Node &range, std::vector<T> &found)
        {
            if (!boundary.IntersectsNode(range))
            {
                return;
            }

            for (auto &item : data)
            {
                if (range.Contains(item->getPos()))
                {
                    found.push_back(item);
                }
            }

            if (children[0] == nullptr)
            {
                return;
            }

            for (auto &child : children)
            {
                child->Query(range, found);
            }

            return;
        }

        void Render()
        {
            boundary.Render();

            if (children[0] == nullptr)
            {
                return;
            }

            for (auto &child : children)
            {
                child->Render();
            }
        }
    };

}
#endif
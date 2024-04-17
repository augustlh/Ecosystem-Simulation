#include "QuadTree.h"
#include <vector>

namespace Ecosim
{

    // bool Node::contains(Vector2<int> point)
    // {
    //     // Checks if the point is within the boundary of the current node
    //     return center.x - width / 2 <= point.x && center.x + width / 2 >= point.x && center.y - width / 2 <= point.y && center.y + width / 2 >= point.y;
    // }

    // Node::~Node() {}

    // template <typename T>
    // QuadTree<T>::~QuadTree() {}

    // template <typename T>
    // void QuadTree<T>::Subdivide()
    // {
    //     // Calculates the new width
    //     float w = boundary.width / 2;

    //     // Calculates the position of the topLeft quadrant
    //     Vector2<int> nw = Vector2<int>(boundary.center.x - w / 2, boundary.center.y - w / 2);

    //     // Creates new child nodes (quadrants) using smart pointers
    //     quadrants[0] = std::make_shared<QuadTree<T>>(Node(nw, w), depth);
    //     quadrants[1] = std::make_shared<QuadTree<T>>(Node(Vector2<int>(nw.x + w, nw.y), w), depth - 1);
    //     quadrants[2] = std::make_shared<QuadTree<T>>(Node(Vector2<int>(nw.x, nw.y + w), w), depth - 1);
    //     quadrants[3] = std::make_shared<QuadTree<T>>(Node(Vector2<int>(nw.x + w, nw.y + w), w), depth - 1);
    // }

    // template <typename T>
    // void QuadTree<T>::Insert(const T &item)
    // {
    //     // If the current node has less than MAX_ITEMS or the depth is 0, insert the item and return
    //     if (data.size() < MAX_ITEMS || depth == 0)
    //     {
    //         data.push_back(item);
    //         return;
    //     }
    //     else
    //     {
    //         // If the quadrants are not initialized, subdivide the current node
    //         if (quadrants[0] == nullptr)
    //         {
    //             Subdivide();
    //         }

    //         // Insert the item into the quadrant that contains the item
    //         for (auto &quadrant : quadrants)
    //         {
    //             if (quadrant != nullptr && quadrant->boundary.contains(item.getPos()))
    //             {
    //                 quadrant->Insert(item);
    //                 return;
    //             }
    //         }
    //     }
    // }

    // template <typename T>
    // void QuadTree<T>::Query(Vector2<int> range, std::vector<T> &found)
    // {
    //     // Check if the range intersects with the boundary of the current node
    //     if (range.x + range.y < boundary.center.x - boundary.width / 2 || range.x - range.y > boundary.center.x + boundary.width / 2 || range.y + range.x < boundary.center.y - boundary.width / 2 || range.y - range.x > boundary.center.y + boundary.width / 2)
    //     {
    //         return;
    //     }

    //     // If the current node is a leaf node (a region that can't be subdivided further), add all items to the found vector
    //     if (quadrants[0] == nullptr)
    //     {
    //         for (const auto &item : data)
    //         {
    //             found.push_back(item);
    //         }
    //     }
    //     else
    //     {
    //         // Recursively query the child nodes (quadrants) for items within the range
    //         for (const auto &quadrant : quadrants)
    //         {
    //             if (quadrant != nullptr)
    //             {
    //                 quadrant->Query(range, found);
    //             }
    //         }
    //     }
    // }

    // template <typename T>
    // void QuadTree<T>::Render()
    // {
    //     // Render the boundary of the current node
    //     boundary.Render();

    //     // Recursively render the child nodes (quadrants)
    //     for (const auto &quadrant : quadrants)
    //     {
    //         if (quadrant != nullptr)
    //         {
    //             quadrant->Render();
    //         }
    //     }
    // }
}

// #include "QuadTree.h"
// #include <vector>

// namespace Ecosim
// {
//     bool Node::contains(Vector2<int> point)
//     {
//         return center.x - width / 2 <= point.x && center.x + width / 2 >= point.x && center.y - width / 2 <= point.y && center.y + width / 2 >= point.y;
//     }

//     Node::~Node() {}

//     QuadTree::~QuadTree()
//     {
//         delete this;
//     }

//     void QuadTree::Insert(const Vector2<int> &point)
//     {
//         if (points.size() < depth)
//         {
//             points.push_back(point);
//             return;
//         }
//         else
//         {
//             if (quadrants[0] == nullptr)
//                 Subdivide();

//             for (auto &quadrant : quadrants)
//             {
//                 if (quadrant != nullptr && quadrant->boundary.contains(point))
//                 {
//                     quadrant->Insert(point);
//                     return;
//                 }
//             }
//         }
//     }

//     void QuadTree::Subdivide()
//     {
//         float x = boundary.center.x;
//         float y = boundary.center.y;
//         float w = boundary.width / 2;

//         Node nw(Vector2<int>(x - w / 2, y - w / 2), w);
//         Node ne(Vector2<int>(x + w / 2, y - w / 2), w);
//         Node sw(Vector2<int>(x - w / 2, y + w / 2), w);
//         Node se(Vector2<int>(x + w / 2, y + w / 2), w);

//         quadrants[0] = std::make_shared<QuadTree>(nw, depth);
//         quadrants[1] = std::make_shared<QuadTree>(ne, depth);
//         quadrants[2] = std::make_shared<QuadTree>(sw, depth);
//         quadrants[3] = std::make_shared<QuadTree>(se, depth);
//     }
//     void QuadTree::Render()
//     {
//         boundary.Render();
//         for (auto &quadrant : quadrants)
//         {
//             if (quadrant != nullptr)
//                 quadrant->Render();
//         }
//     }
// }
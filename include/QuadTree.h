#ifndef QT_H
#define QT_H

#include <memory>
#include <vector>

#include "Vector2.h"
#include "Interfaces.h"

// Implementeringen af QuadTree er inspireret af https://www.youtube.com/watch?v=OJxEcs0w_kE. Dog er den skabt på egen hånd og tilpasset til projektet.

namespace Ecosim
{
    /// @brief A class representing a node. The node is a square with a position and a width i.e. its used to represent a boundary in the quadtree.
    class Node
    {
    private:
        /// @brief The position of the center of the node
        const Vector2<float> m_Pos;

        /// @brief The width of the node (the node is a square)
        const int m_Width;

    public:
        /// @brief Construct a new Node object
        /// @param _pos
        /// @param _width
        Node(const Vector2<float> _pos, const int _width) : m_Pos(_pos), m_Width(_width) {}
        ~Node() = default;

        /// @brief Check if a point is inside the node
        /// @param point The point to check
        /// @return true if the point is inside the node, false if it is not
        bool Contains(Vector2<float> &point);

        /// @brief Check if a node intersects with another node
        /// @param other The other node
        /// @return true if the nodes intersect, false if they do not
        bool IntersectsNode(Node &other);

        /// @brief Render the node (draws the square on the screen)
        void Render();

        /// @brief get the width of the node
        /// @return  The width of the node
        int GetWidth() { return m_Width; }

        /// @brief get the position of the node
        /// @return  The position of the node
        Vector2<float> GetPos() { return m_Pos; }
    };

    /// @brief A class representing a quadtree. The quadtree is used for collision detection in the simulation.
    class QuadTree
    {
    private:
        /// @brief The maximum number of collidables in a node before it subdivides
        const int capacity = 4;

        /// @brief  The boundary node of the quadtree
        Node m_Boundary;

        /// @brief The collidables/objects in the QuadTree
        std::vector<std::shared_ptr<Collidable>> m_Collidables;

        /// @brief The children of the QuadTree. The children are stored in the following order: top-left, top-right, bottom-left, bottom-right.
        std::vector<std::unique_ptr<QuadTree>> m_Children;

    public:
        /// @brief Construct a new QuadTree object
        /// @param _boundary The boundary node of the quadtree
        QuadTree(Node &_boundary) : m_Boundary(_boundary), m_Children(4) {}

        /// @brief Default destructor
        ~QuadTree() = default;

        /// @brief Insert a collidable into the quadtree
        /// @param collidable The collidable to insert
        /// @return true if the collidable was inserted, false if it was not
        bool Insert(std::shared_ptr<Collidable> collidable);

        /// @brief Subdivide the node into 4 children
        void Subdivide();

        /// @brief Query the quadtree for collidables within a certain range
        /// @param range The range to query (A Node)
        /// @param found The vector to store the found collidables in
        void Query(Node &range, std::vector<std::shared_ptr<Collidable>> &found);

        /// @brief Clear the quadtree
        void Clear();

        /// @brief Render the quadtree
        void Render();
    };
}

#endif /* QT_H */
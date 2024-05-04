#include "QuadTree.h"
#include "Renderer.h"

namespace Ecosim
{
    /// @brief Check if this node contains a point
    /// @param point The point to check
    bool Node::Contains(Vector2<float> &point)
    {
        float margin = std::numeric_limits<float>::epsilon() * 2.0f;
        return m_Pos.x - m_Width / 2 <= point.x + margin &&
               m_Pos.x + m_Width / 2 >= point.x - margin &&
               m_Pos.y - m_Width / 2 <= point.y + margin &&
               m_Pos.y + m_Width / 2 >= point.y - margin;
    }

    /// @brief Check if this node intersects with another node
    /// @param other The node to check against
    bool Node::IntersectsNode(Node &other)
    {
        float margin = std::numeric_limits<float>::epsilon() * 2.0f;
        return m_Pos.x - m_Width / 2 <= other.m_Pos.x + other.m_Width / 2 + margin &&
               m_Pos.x + m_Width / 2 >= other.m_Pos.x - other.m_Width / 2 - margin &&
               m_Pos.y - m_Width / 2 <= other.m_Pos.y + other.m_Width / 2 + margin &&
               m_Pos.y + m_Width / 2 >= other.m_Pos.y - other.m_Width / 2 - margin;
    }

    /// @brief Render the node
    void Node::Render()
    {
        Vector2<float> halfSize(static_cast<float>(m_Width) / 2, static_cast<float>(m_Width) / 2);
        Vector2<float> topLeft(m_Pos.x - halfSize.x, m_Pos.y - halfSize.y);
        Vector2<float> topRight(m_Pos.x + halfSize.x, m_Pos.y - halfSize.y);
        Vector2<float> bottomLeft(m_Pos.x - halfSize.x, m_Pos.y + halfSize.y);
        Vector2<float> bottomRight(m_Pos.x + halfSize.x, m_Pos.y + halfSize.y);

        Renderer::Line(topLeft.Convert<int>(), topRight.Convert<int>(), 1, Color(255, 255, 255));
        Renderer::Line(topRight.Convert<int>(), bottomRight.Convert<int>(), 1, Color(255, 255, 255));
        Renderer::Line(bottomRight.Convert<int>(), bottomLeft.Convert<int>(), 1, Color(255, 255, 255));
        Renderer::Line(bottomLeft.Convert<int>(), topLeft.Convert<int>(), 1, Color(255, 255, 255));
    }

    /// @brief Insert a collidable object into the quadtree
    /// @param collidable The collidable object to insert
    bool QuadTree::Insert(std::shared_ptr<Collidable> collidable)
    {
        if (!m_Boundary.Contains(collidable->getPosition()))
        {
            return false;
        }

        if (m_Collidables.size() < QT_NODE_CAPACITY && m_Children[0] == nullptr)
        {
            m_Collidables.push_back(collidable);
            return true;
        }

        if (m_Children[0] == nullptr)
        {
            Subdivide();
        }

        for (auto &child : m_Children)
        {
            if (child->Insert(collidable))
            {
                return true;
            }
        }

        return false;
    }

    /// @brief Subdivide the quadtree into 4 children
    void QuadTree::Subdivide()
    {
        int width = m_Boundary.GetWidth() / 2;

        Vector2<float> pos = m_Boundary.GetPos();

        m_Children[0] = std::make_unique<QuadTree>(Node(Vector2<float>(pos.x - width / 2, pos.y - width / 2), width));
        m_Children[1] = std::make_unique<QuadTree>(Node(Vector2<float>(pos.x + width / 2, pos.y - width / 2), width));
        m_Children[2] = std::make_unique<QuadTree>(Node(Vector2<float>(pos.x - width / 2, pos.y + width / 2), width));
        m_Children[3] = std::make_unique<QuadTree>(Node(Vector2<float>(pos.x + width / 2, pos.y + width / 2), width));
    }

    /// @brief Query the quadtree for collidables within a range
    /// @param range The range to query
    /// @param found The vector to store the found collidables
    void QuadTree::Query(Node &range, std::vector<std::shared_ptr<Collidable>> &found)
    {
        if (!m_Boundary.IntersectsNode(range))
        {
            return;
        }

        for (auto &collidable : m_Collidables)
        {
            if (range.Contains(collidable->getPosition()))
            {
                found.push_back(collidable);
            }
        }

        for (auto &child : m_Children)
        {
            if (child != nullptr)
            {
                child->Query(range, found);
            }
        }
    }

    /// @brief Clear the quadtree. Resets the QuadTree to its initial state
    void QuadTree::Clear()
    {
        m_Collidables.clear();

        for (auto &child : m_Children)
        {
            if (child)
            {
                child->Clear();
            }
        }

        m_Children.clear();
        m_Children.resize(4);
    }

    /// @brief Render the quadtree
    void QuadTree::Render()
    {
        m_Boundary.Render();

        for (auto &child : m_Children)
        {
            if (child != nullptr)
            {
                child->Render();
            }
        }
    }
}
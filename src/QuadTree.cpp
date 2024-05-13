#include "QuadTree.h"
#include "Renderer.h"

// Implementeringen af QuadTree er inspireret af https://www.youtube.com/watch?v=OJxEcs0w_kE. Dog er den skabt på egen hånd og tilpasset til projektet.

namespace Ecosim
{
    bool Node::Contains(Vector2<float> &point)
    {
        float margin = std::numeric_limits<float>::epsilon() * 2.0f;
        return m_Pos.x - m_Width / 2 <= point.x + margin &&
               m_Pos.x + m_Width / 2 >= point.x - margin &&
               m_Pos.y - m_Width / 2 <= point.y + margin &&
               m_Pos.y + m_Width / 2 >= point.y - margin;
    }

    bool Node::IntersectsNode(Node &other)
    {
        float margin = std::numeric_limits<float>::epsilon() * 2.0f;
        return m_Pos.x - m_Width / 2 <= other.m_Pos.x + other.m_Width / 2 + margin &&
               m_Pos.x + m_Width / 2 >= other.m_Pos.x - other.m_Width / 2 - margin &&
               m_Pos.y - m_Width / 2 <= other.m_Pos.y + other.m_Width / 2 + margin &&
               m_Pos.y + m_Width / 2 >= other.m_Pos.y - other.m_Width / 2 - margin;
    }

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

    bool QuadTree::Insert(std::shared_ptr<Collidable> collidable)
    {
        if (!m_Boundary.Contains(collidable->getPosition()))
        {
            return false;
        }

        if (m_Collidables.size() < capacity && m_Children[0] == nullptr)
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

    void QuadTree::Subdivide()
    {
        int width = m_Boundary.GetWidth() / 2;

        Vector2<float> pos = m_Boundary.GetPos();

        m_Children[0] = std::make_unique<QuadTree>(Node(Vector2<float>(pos.x - width / 2, pos.y - width / 2), width));
        m_Children[1] = std::make_unique<QuadTree>(Node(Vector2<float>(pos.x + width / 2, pos.y - width / 2), width));
        m_Children[2] = std::make_unique<QuadTree>(Node(Vector2<float>(pos.x - width / 2, pos.y + width / 2), width));
        m_Children[3] = std::make_unique<QuadTree>(Node(Vector2<float>(pos.x + width / 2, pos.y + width / 2), width));
    }

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
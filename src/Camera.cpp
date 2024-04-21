#include "Camera.h"
#include <algorithm>

namespace Ecosim
{
    Vector2<float> Camera::m_position{};
    Vector2<float> Camera::m_viewport{};
    float Camera::m_zoom = 1.0f;

    void Camera::SetViewport(const Vector2<float> &viewport) { m_viewport = viewport; }

    void Camera::SetPosition(const Vector2<float> &pos) { m_position = pos; }
    void Camera::MovePosition(const Vector2<float> &move) { m_position += move; }

    void Camera::Zoom(float amount) { SetZoom(m_zoom - amount); }
    void Camera::SetZoom(float zoom)
    {
        zoom = std::clamp(zoom, 0.125f, 8.0f);

        // Vector2<float> displacement = (m_viewport / 2) * zoom + m_position - ((m_viewport / 2) * m_zoom + m_position);
        Vector2<float> displacement = m_viewport * ((zoom - m_zoom) / 2.0f);
        m_position -= displacement;

        m_zoom = zoom;
    }

    Vector2<float> Camera::TranslatePoint(const Vector2<float> &pos) { return (pos - m_position) / m_zoom; }

    float Camera::TranslateDistance(float dist) { return dist / m_zoom; }

    SDL_Rect Camera::TranslateRect(const Vector2<float> &pos, const Vector2<float> &size)
    {
        Vector2<int> translated = TranslatePoint(pos).Convert<int>();
        return {translated.x, translated.y, static_cast<int>(TranslateDistance(size.x)), static_cast<int>(TranslateDistance(size.y))};
    }

    Vector2<float> Camera::ViewportToWorld(const Vector2<float> &pos)
    {
        return pos * m_zoom + m_position;
    }
}

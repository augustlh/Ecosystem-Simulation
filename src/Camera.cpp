#include "Camera.h"
#include <algorithm>

namespace Ecosim
{
    Vector2<float> Camera::m_position{};
    Vector2<float> Camera::m_viewport{};
    float Camera::m_zoom = 1.0f;

    void Camera::SetViewport(Vector2<float> &viewport) { m_viewport = viewport; }

    void Camera::SetPosition(const Vector2<float> &pos) { m_position = pos; }
    void Camera::MovePosition(const Vector2<float> &move) { m_position += move; }

    void Camera::Zoom(float amount) { SetZoom(m_zoom - amount); } // TODO: Value clamping
    void Camera::SetZoom(float zoom)
    {
        zoom = std::clamp(zoom, 0.125f, 8.0f);

        // (viewport / 2) * m_zoom + m_position;
        // (viewport / 2) *   zoom + m_position;

        Vector2<float> displacement = (m_viewport / 2) * zoom + m_position - ((m_viewport / 2) * m_zoom + m_position);
        m_position -= displacement;

        // float zoomRatio = zoom / m_zoom;
        // m_position -= m_position * zoomRatio;

        // ...
        m_zoom = zoom;
    }

    // Vector2<float> Camera::ScreenToWorld(const Vector2<float> &screenPos)
    // {
    //     int width = 800; // Assuming width and height are your viewport dimensions
    //     int height = 800;

    //     // Calculate the position of the center of the screen in world coordinates
    //     Vector2<float> centerWorldPos(m_position.x + width / 2, m_position.y + height / 2);

    //     // Calculate the offset from the center of the screen to the given screen position
    //     Vector2<float> offsetFromCenter(screenPos.x - width / 2, screenPos.y - height / 2);

    //     // Apply the inverse of zoom to get the world coordinates
    //     Vector2<float> worldPos = centerWorldPos + (offsetFromCenter / m_zoom);

    //     return worldPos;
    // }

    Vector2<float> Camera::TranslatePoint(const Vector2<float> &pos)
    {
        // |x - cam| = zoom * |pos - cam|
        // a = x - cam, b = pos - cam
        // x = zoom * |b| * norm(b) + cam

        // Vector2<float> viewport{800.0f, 800.0f};
        float zoom = std::clamp(m_zoom, 0.1f, 10.0f);
        return (pos - m_position) / zoom;
        // return Vector2<float>(m_zoom * pos.x - m_position.x, m_zoom * pos.y - m_position.y);
    }

    // float Camera::TranslateDistance(float dist) { return dist * m_zoom; }
    float Camera::TranslateDistance(float dist)
    {
        // Vector2<float> viewport{800.0f, 800.0f};
        float zoom = std::clamp(m_zoom, 0.1f, 10.0f);
        return dist / zoom;
    }

    SDL_Rect Camera::TranslateRect(const Vector2<float> &pos, const Vector2<float> &size)
    {
        Vector2<int> translated = TranslatePoint(pos).Convert<int>();
        return {translated.x, translated.y, static_cast<int>(TranslateDistance(size.x)), static_cast<int>(TranslateDistance(size.y))};
    }
}

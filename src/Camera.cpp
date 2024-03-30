#include "Camera.h"

namespace Ecosim
{
    Vector2<float> Camera::m_position{};
    float Camera::m_zoom = 1.0f;

    void Camera::SetPosition(const Vector2<float> &pos) { m_position = pos; }

    void Camera::MovePosition(const Vector2<float> &move) { m_position += move; }

    void Camera::SetZoom(float zoom) { m_zoom = zoom; }

    void Camera::Zoom(float amount) { m_zoom += amount; } // TODO: Value clamping

    Vector2<float> Camera::TranslatePoint(const Vector2<float> &pos)
    {
        // |x - cam| = zoom * |pos - cam|
        // a = x - cam, b = pos - cam
        // x = zoom * |b| * norm(b) + cam

        float magnitude = (pos - m_position).Magnitude();
        if (magnitude > 0.0f)
        {
            Vector2<float> posNorm = pos / magnitude;
            return Vector2<float>(m_zoom * magnitude * posNorm.x - m_position.x, m_zoom * magnitude * posNorm.y - m_position.y);
        }
        return m_position;
    }

    float Camera::TranslateDistance(float dist) { return dist * m_zoom; }

    SDL_Rect Camera::TranslateRect(const Vector2<float> &pos, const Vector2<float> &size)
    {
        Vector2<int> translated = TranslatePoint(pos).Convert<int>();
        return {translated.x, translated.y, static_cast<int>(size.x * m_zoom), static_cast<int>(size.y * m_zoom)};
    }
}

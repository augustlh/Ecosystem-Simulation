#ifndef WORLD_H
#define WORLD_H

#include "Vector2.h"
#include "SDL3/SDL.h"

namespace Ecosim
{
    class Camera
    {
    private:
        static Vector2<float> m_position;
        static Vector2<float> m_viewport;
        static float m_zoom;

    public:
        Camera() = delete;

        static void SetViewport(Vector2<float> &viewport);

        static void SetPosition(const Vector2<float> &pos);
        static void MovePosition(const Vector2<float> &move);

        static void SetZoom(float zoom);
        static void Zoom(float amount);

        // static Vector2<float> ScreenToWorld(const Vector2<float> &screenPos);

        static Vector2<int> TranslatePoint(const Vector2<int> &pos) { return TranslatePoint(pos.Convert<float>()).Convert<int>(); }
        static Vector2<float> TranslatePoint(const Vector2<float> &pos);
        static int TranslateDistance(int dist) { return static_cast<int>(TranslateDistance(static_cast<float>(dist))); }
        static float TranslateDistance(float dist);
        static SDL_Rect TranslateRect(const Vector2<int> &pos, const Vector2<int> &size) { return TranslateRect(pos.Convert<float>(), size.Convert<float>()); }
        static SDL_Rect TranslateRect(const Vector2<float> &pos, const Vector2<float> &size);
    };
}

#endif /* WORLD_H */
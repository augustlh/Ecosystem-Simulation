#ifndef CAMERA_H
#define CAMERA_H

#include "Ecosim.h"
#include "Vector2.h"

namespace Ecosim
{
    class Camera
    {
    private:
        /// @brief The position of the camera in world coordinates
        ECOSIM_API static Vector2<float> m_position;

        /// @brief The size of the viewport (window) for correct zooming
        ECOSIM_API static Vector2<float> m_viewport;

        /// @brief The current zoom value (large value = large zoom)
        ECOSIM_API static float m_zoom;

    public:
        /// @brief Delete the constructor to prevent creating a instance of the Camera-class
        Camera() = delete;

        /// @brief Set the viewport (window) size
        /// @param viewport A Vector2-object with x=width and y=height
        ECOSIM_API static void SetViewport(const Vector2<float> &viewport);

        /// @brief Set the cameras position in world space
        /// @param pos The worldspace coordinate
        ECOSIM_API static void SetPosition(const Vector2<float> &pos);

        /// @brief Offset the cameras position in world space
        /// @param move The worldspace offset relative to the current camera position
        ECOSIM_API static void MovePosition(const Vector2<float> &move);

        /// @brief Set the camera zoom. The value is clamped between 0.125 and 8.0
        /// @param zoom The zoom value
        ECOSIM_API static void SetZoom(float zoom);

        /// @brief Zoom the camera in or out relative to the current zoom
        /// @param amount The relative zoom amount
        ECOSIM_API static void Zoom(float amount);

        /// @brief Translates an int-point from worldspace to viewport-space
        /// @param pos The worldspace coordinate
        /// @return The viewport coordinate
        ECOSIM_API static Vector2<int> TranslatePoint(const Vector2<int> &pos) { return TranslatePoint(pos.Convert<float>()).Convert<int>(); }

        /// @brief Translates a float-point from worldspace to viewport-space
        /// @param pos The worldspace coordinate
        /// @return The viewport coordinate
        ECOSIM_API static Vector2<float> TranslatePoint(const Vector2<float> &pos);

        /// @brief Translates an int-distance from worldspace to viewport-space
        /// @param dist The worldspace distance
        /// @return The viewport distance
        ECOSIM_API static int TranslateDistance(int dist) { return static_cast<int>(TranslateDistance(static_cast<float>(dist))); }

        /// @brief Translates a float-distance from worldspace to viewport-space
        /// @param dist The worldspace distance
        /// @return The viewport distance
        ECOSIM_API static float TranslateDistance(float dist);

        /// @brief Translates an int-rectangle in worldspace to viewport-space
        /// @param pos The worldspace upper left corner of the rect
        /// @param size The worldspace size where x=width, y=height
        /// @return An SDL_Rect in viewport space
        ECOSIM_API static SDL_Rect TranslateRect(const Vector2<int> &pos, const Vector2<int> &size) { return TranslateRect(pos.Convert<float>(), size.Convert<float>()); }

        /// @brief Translates a float-rectangle in worldspace to viewport-space
        /// @param pos The worldspace upper left corner of the rect
        /// @param size The worldspace size where x=width, y=height
        /// @return An SDL_Rect in viewport space
        ECOSIM_API static SDL_Rect TranslateRect(const Vector2<float> &pos, const Vector2<float> &size);

        /// @brief Translates an int-viewport coordinate into a worldspace coordinate
        /// @param pos The viewport coordinate
        /// @return The worldspace coordinate
        ECOSIM_API static Vector2<int> ViewportToWorld(const Vector2<int> &pos) { return ViewportToWorld(pos.Convert<float>()).Convert<int>(); }

        /// @brief Translates a float-viewport coordinate into a worldspace coordinate
        /// @param pos The viewport coordinate
        /// @return The worldspace coordinate
        ECOSIM_API static Vector2<float> ViewportToWorld(const Vector2<float> &pos);
    };
}

#endif /* CAMERA_H */
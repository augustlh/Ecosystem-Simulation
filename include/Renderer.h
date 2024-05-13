#ifndef RENDERER_H
#define RENDERER_H

#include "Ecosim.h"
#include "Vector2.h"
#include <cstdint>
#include <sstream>

namespace Ecosim
{
    /// @brief A struct representing a RGB-color
    struct ECOSIM_API Color
    {
        /// @brief The red, green, and blue component of the color
        uint8_t r, g, b;

        /// @brief Create a color with RGB=(0,0,0)
        Color() : r(0), g(0), b(0) {}

        /// @brief Create a color with RGB=(_c,_c,_c)
        /// @param _c The grayscale value
        Color(uint8_t _c) : r(_c), g(_c), b(_c) {}

        /// @brief Create a color
        /// @param _r The red component
        /// @param _g The green component
        /// @param _b The blue component
        Color(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}

        /// @brief Formats the color as a unsigned 4-byte integer
        /// @param format A pointer to an `SDL_PixelFormat`
        /// @return An SDL-accepted color
        uint32_t Format(SDL_PixelFormat *format) { return SDL_MapRGB(format, r, g, b); }

        /// @brief Creates a color-object from a hex string
        /// @param hex The hex string, which must be of the format: "#rrggbb"
        /// @return A color-object
        static Color FromHex(const std::string &hex);
    };

    /// @brief A class for rendering basic shapes and textures to the screen
    class ECOSIM_API Renderer
    {
    private:
        /// @brief The SDL window to renderer to
        static SDL_Window *m_sdlWindow;

        /// @brief The windows drawing surface
        static SDL_Surface *m_sdlSurface;

    private:
        /// @brief Draw a horizontal line to the screen from point `(x0, y)` to `(x1, y)`
        /// @param x0
        /// @param x1
        /// @param y
        /// @param color The color of the line
        static void HLine(int x0, int x1, int y, Color color);

    public:
        /// @brief Gets called by the `Window`-class when a new window have been created
        /// @param sdlWindow A pointer to the created window
        static void SetupSDLRenderSurface(SDL_Window *sdlWindow);

        /// @brief Requests a drawable SDL_Surface which can be rendered to the screen
        /// @param width The width of the surface
        /// @param height The height of the surface
        /// @return The SDL_Surface
        static SDL_Surface *RequestSurface(int width, int height);

        /// @brief Renders a frame
        static void RenderFrame();

        /// @brief Fills the screen with a single color
        /// @param color The color to fill
        static void Background(Color color);

        /// @brief Draw a rectangle to the screen
        /// @param x The upper left x-coordinate of the rect
        /// @param y The upper left y-coordinate of the rect
        /// @param w The width of the rect
        /// @param h The height of the rect
        /// @param color The fill-color of the rect
        static void Rect(int x, int y, int w, int h, Color color);

        /// @brief Draw a rectangle to the screen
        /// @param pos The upper left corner of the rect
        /// @param size The size of the rect
        /// @param color The fill-color of the rect
        static void Rect(const Vector2<int> &pos, const Vector2<int> &size, Color color);

        /// @brief Draws a circle to the screen
        /// @param x The center x-coordinate
        /// @param y The center y-coordinate
        /// @param r The radius of the circle
        /// @param color The fill-color of the circle
        static void Circle(int x, int y, int r, Color color);

        /// @brief Draws a circle to the screen
        /// @param pos The center of the circle
        /// @param r The radius of the circle
        /// @param color The fill-color of the circle
        static void Circle(const Vector2<int> &pos, int r, Color color);

        /// @brief Draws a line to the screen from `(x0, y0)` to `(x1, y1)`
        /// @param x0
        /// @param y0
        /// @param x1
        /// @param y1
        /// @param size The width of the line
        /// @param color The fill-color of the line
        static void Line(int x0, int y0, int x1, int y1, int size, Color color);

        /// @brief Draws a line to the screen
        /// @param pos0 The from-coordinate
        /// @param pos1 The to-coordinate
        /// @param size The width of the line
        /// @param color The fill-color of the line
        static void Line(const Vector2<int> &pos0, const Vector2<int> &pos1, int size, Color color);

        /// @brief Draws an SDL_Surface to the screen
        /// @param x The upper left x-coordinate of the surface
        /// @param y The upper left y-coordinate of the surface
        /// @param surface The surface to draw
        static void Surface(int x, int y, SDL_Surface *surface);

        /// @brief Draws an SDL_Surface to the screen
        /// @param pos The upper left corner of the surface
        /// @param surface The surface to draw
        static void Surface(const Vector2<int> &pos, SDL_Surface *surface);
    };
}

#endif /* RENDERER_H */
#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>
#include <cstdint>
#include <sstream>
#include "Vector2.h"

typedef unsigned int uint;

namespace Ecosim
{
    struct Color
    {
        uint8_t r, g, b;

        Color() : r(0), g(0), b(0) {}
        Color(uint8_t _c) : r(_c), g(_c), b(_c) {}
        Color(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}

        uint32_t Format(SDL_PixelFormat *format) { return SDL_MapRGB(format, r, g, b); }

        static Color FromHex(const std::string &hex);
    };

    class Renderer
    {
    private:
        static SDL_Window *m_sdlWindow;
        static SDL_Surface *m_sdlSurface;

    private:
        static void HLine(int x0, int x1, int y, Color color);

    public:
        static void SetupSDLRenderSurface(SDL_Window *sdlWindow);

        static SDL_Surface *RequestSurface(int width, int height);

        static void RenderFrame();

        static void Background(Color color);
        static void Rect(int x, int y, int w, int h, Color color);
        static void Rect(const Vector2<int> &pos, const Vector2<int> &size, Color color);
        static void Circle(int x, int y, int r, Color color);
        static void Circle(const Vector2<int> &pos, int r, Color color);
        static void Line(int x0, int y0, int x1, int y1, int size, Color color);
        static void Line(const Vector2<int> &pos0, const Vector2<int> &pos1, int size, Color color);
        static void Surface(int x, int y, SDL_Surface *surface);
        static void Surface(const Vector2<int> &pos, SDL_Surface *surface);
    };
}

#endif /* RENDERER_H */
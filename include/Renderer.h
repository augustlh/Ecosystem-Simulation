#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <cstdint>

#include "Vector2.h"

namespace Ecosim
{
    struct Color
    {
        uint8_t r, g, b, a;
        Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) : r(_r), g(_g), b(_b), a(_a) {}
    };

    class Renderer
    {
    private:
        static SDL_Renderer *m_sdlRenderer;

    public:
        Renderer() = delete;

        static void SetSDLRenderer(SDL_Renderer *sdlRenderer);

        static void RenderFrame();

        static SDL_Surface *RequestSDLSurface(Vector2<int> size);
        static SDL_Surface *RequestSDLSurface(int width, int height);
        static SDL_Texture *BakeTexture(SDL_Surface *surface);

        static void Background(Color color);
        static void Circle(Vector2<int> center, int radius, Color color);
        static void Line(Vector2<int> from, Vector2<int> to, Color color);
        static void Texture(SDL_Texture *texture, Vector2<int> pos);
    };
}

#endif /* RENDERER_H */
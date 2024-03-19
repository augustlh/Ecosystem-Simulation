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
    };

    class Renderer
    {
    private:
        static SDL_Renderer *m_sdl_renderer;

    public:
        static void SetSDLRenderer(SDL_Renderer *sdl_renderer);

        static void RenderFrame();

        static void Background(Color color);
        static void Circle(Vector2i center, int radius, Color color);
    };
}

#endif /* RENDERER_H */
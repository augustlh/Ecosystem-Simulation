#include "Renderer.h"
#include <iostream>

namespace Ecosim
{
    SDL_Renderer *Renderer::m_sdlRenderer = nullptr;

    void Renderer::SetSDLRenderer(SDL_Renderer *sdlRenderer) { Renderer::m_sdlRenderer = sdlRenderer; }

    void Renderer::RenderFrame()
    {
        SDL_RenderPresent(Renderer::m_sdlRenderer);
    }

    void Renderer::Background(Color color)
    {
        SDL_SetRenderDrawColor(Renderer::m_sdlRenderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(Renderer::m_sdlRenderer);
    }

    void Renderer::Circle(Vector2i center, int radius, Color color)
    {
        filledCircleRGBA(Renderer::m_sdlRenderer, (int16_t)center.x, (int16_t)center.y, (int16_t)radius, color.r, color.g, color.b, color.a);
    }

    void Renderer::Line(Vector2i from, Vector2i to, Color color)
    {
        lineRGBA(Renderer::m_sdlRenderer, from.x, from.y, to.x, to.y, color.r, color.g, color.b, color.a);
    }
}

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

    SDL_Surface *Renderer::RequestSDLSurface(Vector2<int> size) { return RequestSDLSurface(size.x, size.y); }

    SDL_Surface *Renderer::RequestSDLSurface(int width, int height)
    {
        return SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    }

    SDL_Texture *Renderer::BakeTexture(SDL_Surface *surface)
    {
        return SDL_CreateTextureFromSurface(m_sdlRenderer, surface);
    }

    void Renderer::Background(Color color)
    {
        SDL_SetRenderDrawColor(Renderer::m_sdlRenderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(Renderer::m_sdlRenderer);
    }

    void Renderer::Circle(Vector2<int> center, int radius, Color color)
    {
        filledCircleRGBA(Renderer::m_sdlRenderer, (int16_t)center.x, (int16_t)center.y, (int16_t)radius, color.r, color.g, color.b, color.a);
    }

    void Renderer::Line(Vector2<int> from, Vector2<int> to, Color color)
    {
        lineRGBA(Renderer::m_sdlRenderer, from.x, from.y, to.x, to.y, color.r, color.g, color.b, color.a);
    }

    void Renderer::Texture(SDL_Texture *texture, Vector2<int> pos)
    {
        int textureWidth, textureHeight;
        SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
        SDL_Rect destination = {.x = pos.x, .y = pos.y, .w = textureWidth, .h = textureHeight};
        SDL_RenderCopy(m_sdlRenderer, texture, NULL, &destination);
    }
}

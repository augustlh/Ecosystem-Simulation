#include "Renderer.h"
#include <iostream>

namespace Ecosim
{
    SDL_Renderer *Renderer::m_sdl_renderer = nullptr;

    void Renderer::SetSDLRenderer(SDL_Renderer *sdl_renderer)
    {
        Renderer::m_sdl_renderer = sdl_renderer;
        std::cout << "SDL Renderer from Renderer: " << Renderer::m_sdl_renderer << std::endl;
    }

    void Renderer::RenderFrame()
    {
        SDL_RenderPresent(Renderer::m_sdl_renderer);
    }

    // void Renderer::Background(Color color)
    // {
    //     SDL_SetRenderDrawColor(Renderer::m_sdl_renderer, color.r, color.g, color.b, color.a);
    //     SDL_RenderClear(Renderer::m_sdl_renderer);
    // }

    // void Renderer::Circle(Vector2i center, int radius, Color color)
    // {
    //     filledCircleRGBA(Renderer::m_sdl_renderer, (int16_t)center.x, (int16_t)center.y, (int16_t)radius, color.r, color.g, color.b, color.a);
    // }

    void Renderer::Background(Color color)
    {
        if (SDL_SetRenderDrawColor(Renderer::m_sdl_renderer, color.r, color.g, color.b, color.a) != 0)
        {
            std::cerr << "Failed to set render draw color: " << SDL_GetError() << std::endl;
            // Handle error appropriately, such as returning or throwing an exception
        }

        if (SDL_RenderClear(Renderer::m_sdl_renderer) != 0)
        {
            std::cerr << "Failed to clear renderer: " << SDL_GetError() << std::endl;
            // Handle error appropriately, such as returning or throwing an exception
        }
    }

    void Renderer::Circle(Vector2i center, int radius, Color color)
    {
        if (filledCircleRGBA(Renderer::m_sdl_renderer, (int16_t)center.x, (int16_t)center.y, (int16_t)radius, color.r, color.g, color.b, color.a) != 0)
        {
            std::cerr << "Failed to draw circle: " << SDL_GetError() << std::endl;
            // Handle error appropriately, such as returning or throwing an exception
        }
    }

}

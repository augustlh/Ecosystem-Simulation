#include "Map.h"
#include "Renderer.h"

// #include <vector>
#include <cstring>
#include <iostream>
#include <random>

namespace Ecosim
{
    void SetPixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
    {
        uint8_t *pixelAddr = (uint8_t *)surface->pixels + y * surface->pitch + x * sizeof(uint32_t);
        *(uint32_t *)pixelAddr = pixel;
    }

    Map::Map()
    {
        SDL_Surface *surface = Renderer::RequestSDLSurface(800, 800);

        SDL_LockSurface(surface);
        for (int x = 0; x < surface->w; ++x)
        {
            for (int y = 0; y < surface->h; ++y)
            {
                uint32_t color = SDL_MapRGB(surface->format, rand() % 160, rand() % 180, rand() % 255);
                SetPixel(surface, x, y, color);
            }
        }
        // SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 51, 77, 102));
        SDL_UnlockSurface(surface);

        m_texture = Renderer::BakeTexture(surface);
        SDL_FreeSurface(surface);
    }

    void Map::Render()
    {
        Renderer::Texture(m_texture, Vector2<int>(50, 20));
    }

    Map::~Map()
    {
        SDL_DestroyTexture(m_texture);
    }
}
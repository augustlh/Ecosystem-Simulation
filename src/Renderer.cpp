#include "Renderer.h"
#include <algorithm>

namespace Ecosim
{
    SDL_Window *Renderer::m_sdlWindow = nullptr;
    SDL_Surface *Renderer::m_sdlSurface = nullptr;

    void Renderer::HLine(int x0, int x1, int y, Color color)
    {
        if (x1 < x0)
            std::swap(x0, x1);

        SDL_Rect rect = {x0, y, x1 - x0, 1};
        uint32_t sdlColor = SDL_MapRGB(m_sdlSurface->format, color.r, color.g, color.b);
        SDL_FillSurfaceRect(m_sdlSurface, &rect, sdlColor);
    }

    void Renderer::Renderer::SetupSDLRenderSurface(SDL_Window *sdlWindow)
    {
        m_sdlWindow = sdlWindow;
        m_sdlSurface = SDL_GetWindowSurface(sdlWindow);
    }

    // SDL_Surface *Renderer::RequestSurface(int width, int height) { return SDL_CreateSurface(0, width, height, 32, 0, 0, 0, 0); }
    SDL_Surface *Renderer::RequestSurface(int width, int height) { return SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBX32); }

    void Renderer::RenderFrame() { SDL_UpdateWindowSurface(m_sdlWindow); }

    void Renderer::Background(Color color)
    {
        uint32_t sdlColor = SDL_MapRGB(m_sdlSurface->format, color.r, color.g, color.b);
        SDL_FillSurfaceRect(m_sdlSurface, &m_sdlSurface->clip_rect, sdlColor);
    }

    void Renderer::Rect(const Vector2<int> &pos, const Vector2<int> &size, Color color) { Rect(pos.x, pos.y, size.x, size.y, color); }
    void Renderer::Rect(int x, int y, int w, int h, Color color)
    {
        SDL_Rect rect = {x, y, w, h};
        uint32_t sdlColor = SDL_MapRGB(m_sdlSurface->format, color.r, color.g, color.b);
        SDL_FillSurfaceRect(m_sdlSurface, &rect, sdlColor);
    }

    void Renderer::Circle(const Vector2<int> &pos, int r, Color color) { Circle(pos.x, pos.y, r, color); }
    void Renderer::Circle(int x, int y, int r, Color color)
    {
        // https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
        int t1 = r >> 4, t2 = 0;
        int dx = r, dy = 0;

        while (dx >= dy)
        {
            HLine(x - dx, x + dx, y + dy, color);
            HLine(x - dx, x + dx, y - dy, color);
            HLine(x - dy, x + dy, y + dx, color);
            HLine(x - dy, x + dy, y - dx, color);

            dy += 1;
            t1 += dy;
            t2 = t1 - dx;

            if (t2 >= 0)
            {
                t1 = t2;
                dx -= 1;
            }
        }
    }

    void Renderer::Line(const Vector2<int> &pos0, const Vector2<int> &pos1, int size, Color color) { Line(pos0.x, pos0.y, pos1.x, pos1.y, size, color); }
    void Renderer::Line(int x0, int y0, int x1, int y1, int size, Color color)
    {
        // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
        int dx = std::abs(x1 - x0);
        int sx = x0 < x1 ? 1 : -1;
        int dy = -std::abs(y1 - y0);
        int sy = y0 < y1 ? 1 : -1;

        int error = dx + dy, e2 = 0;

        uint32_t sdlColor = SDL_MapRGB(m_sdlSurface->format, color.r, color.g, color.b);
        SDL_Rect point = {0, 0, size, size};

        while (true)
        {
            point.x = x0 - (size >> 2);
            point.y = y0 - (size >> 2);
            SDL_FillSurfaceRect(m_sdlSurface, &point, sdlColor);

            if (x0 == x1 && y0 == y1)
                break;

            e2 = error * 2;
            if (e2 >= dy)
            {
                if (x0 == x1)
                    break;
                error += dy;
                x0 += sx;
            }
            if (e2 <= dx)
            {
                if (y0 == y1)
                    break;
                error += dx;
                y0 += sy;
            }
        }
    }

    void Renderer::Surface(const Vector2<int> &pos, SDL_Surface *surface) { Surface(pos.x, pos.y, surface); }
    void Renderer::Surface(int x, int y, SDL_Surface *surface)
    {
        SDL_Rect rect = {x, y, surface->w, surface->h};
        SDL_BlitSurface(surface, NULL, m_sdlSurface, &rect);
    }
}
#include "Renderer.h"
#include "Camera.h"
#include <algorithm>
#include <iostream>

namespace Ecosim
{
    Color Color::FromHex(const std::string &hex)
    {
        std::stringstream ss(hex.substr(1));

        uint32_t num = 0;
        ss >> std::hex >> num;

        uint8_t r = (num >> 16) & 0xff;
        uint8_t g = (num >> 8) & 0xff;
        uint8_t b = num & 0xff;

        return Color(r, g, b);
    }

    SDL_Window *Renderer::m_sdlWindow = nullptr;
    SDL_Surface *Renderer::m_sdlSurface = nullptr;

    void Renderer::HLine(int x0, int x1, int y, Color color)
    {
        if (x1 < x0)
            std::swap(x0, x1);

        SDL_Rect rect = {x0, y, x1 - x0, 1};
        SDL_FillSurfaceRect(m_sdlSurface, &rect, color.Format(m_sdlSurface->format));
    }

    void Renderer::Renderer::SetupSDLRenderSurface(SDL_Window *sdlWindow)
    {
        m_sdlWindow = sdlWindow;
        m_sdlSurface = SDL_GetWindowSurface(sdlWindow);
    }

    SDL_Surface *Renderer::RequestSurface(int width, int height) { return SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBX32); }

    void Renderer::RenderFrame() { SDL_UpdateWindowSurface(m_sdlWindow); }

    void Renderer::Background(Color color)
    {
        SDL_FillSurfaceRect(m_sdlSurface, &m_sdlSurface->clip_rect, color.Format(m_sdlSurface->format));
    }

    void Renderer::Rect(const Vector2<int> &pos, const Vector2<int> &size, Color color) { Rect(pos.x, pos.y, size.x, size.y, color); }
    void Renderer::Rect(int x, int y, int w, int h, Color color)
    {
        SDL_Rect rect = Camera::TranslateRect(Vector2<int>(x, y), Vector2<int>(w, h)); // TODO: make TranslateRect(int, int, int, int)
        SDL_FillSurfaceRect(m_sdlSurface, &rect, color.Format(m_sdlSurface->format));
    }

    void Renderer::Circle(const Vector2<int> &pos, int r, Color color) { Circle(pos.x, pos.y, r, color); }
    void Renderer::Circle(int x, int y, int r, Color color)
    {
        // https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
        Vector2<int> center = Camera::TranslatePoint(Vector2<int>(x, y));
        r = Camera::TranslateDistance(r);

        int t1 = r >> 4, t2 = 0;
        int dx = r, dy = 0;

        while (dx >= dy)
        {
            HLine(center.x - dx, center.x + dx, center.y + dy, color);
            HLine(center.x - dx, center.x + dx, center.y - dy, color);
            HLine(center.x - dy, center.x + dy, center.y + dx, color);
            HLine(center.x - dy, center.x + dy, center.y - dx, color);

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

        Vector2<int> p0 = Camera::TranslatePoint(Vector2<int>(x0, y0));
        Vector2<int> p1 = Camera::TranslatePoint(Vector2<int>(x1, y1));

        int dx = std::abs(p1.x - p0.x);
        int sx = p0.x < p1.x ? 1 : -1;
        int dy = -std::abs(p1.y - p0.y);
        int sy = p0.y < p1.y ? 1 : -1;

        int error = dx + dy, e2 = 0;

        uint32_t sdlColor = color.Format(m_sdlSurface->format);
        SDL_Rect point = {0, 0, size, size};

        while (true)
        {
            point.x = p0.x - (size >> 2);
            point.y = p0.y - (size >> 2);
            SDL_FillSurfaceRect(m_sdlSurface, &point, sdlColor);

            if (p0.x == p1.x && p0.y == p1.y)
                break;

            e2 = error * 2;
            if (e2 >= dy)
            {
                if (p0.x == p1.x)
                    break;
                error += dy;
                p0.x += sx;
            }
            if (e2 <= dx)
            {
                if (p0.y == p1.y)
                    break;
                error += dx;
                p0.y += sy;
            }
        }
    }

    void Renderer::Surface(const Vector2<int> &pos, SDL_Surface *surface) { Surface(pos.x, pos.y, surface); }
    void Renderer::Surface(int x, int y, SDL_Surface *surface)
    {
        SDL_Rect rect = Camera::TranslateRect(Vector2<int>(x, y), Vector2<int>(surface->w, surface->h));
        // SDL_BlitSurface(surface, NULL, m_sdlSurface, &rect);
        SDL_BlitSurfaceScaled(surface, NULL, m_sdlSurface, &rect, SDL_SCALEMODE_NEAREST);
    }
}
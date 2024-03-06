#include "Window.h"
#include <iostream>

Window::Window(const char *title, unsigned int width, unsigned int height)
    : m_window(nullptr), m_renderer(nullptr), m_surface(nullptr), m_width(width), m_height(height), width(m_width), height(m_height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to initialize: %s", SDL_GetError());
        return;
    }

    m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, SDL_WINDOW_SHOWN);
    if (NULL == m_window)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL window: %s", SDL_GetError());
        return;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (NULL == m_renderer)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL renderer: %s", SDL_GetError());
        return;
    }

    // m_surface = SDL_CreateRGBSurface(0, m_width, m_height, 32, 0, 0, 0, 0);
    // if (NULL == m_surface)
    // {
    //     SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL surface: %s", SDL_GetError());
    //     return;
    // }
}

Window::~Window()
{
    // SDL_FreeSurface(m_surface);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Window::Renderer()
{
    for (int i = 0; i < (int)m_width; ++i)
    {
        for (int j = 0; j < (int)m_height; ++j)
        {
            SDL_SetRenderDrawColor(m_renderer, i % 255, j % 255, 0, 255);
            SDL_RenderDrawPoint(m_renderer, i, j);
        }
    }
    SDL_SetRenderDrawColor(m_renderer, 51, 77, 102, 255);

    for (int i = -10; i < 10; ++i)
        SDL_RenderDrawLine(m_renderer, i, 0, m_width + i, m_height);

    SDL_RenderPresent(m_renderer);
}
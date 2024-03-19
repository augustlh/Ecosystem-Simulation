#include "Window.h"
#include "Renderer.h"
#include <iostream>

namespace Ecosim
{
    Window::Window(const char *title, unsigned int width, unsigned int height)
        : m_window(nullptr), m_renderer(nullptr), m_width(width), m_height(height), width(m_width), height(m_height)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to initialize: %s", SDL_GetError());
            return;
        }

        m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, SDL_WINDOW_SHOWN);
        if (m_window == NULL)
        {
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL window: %s", SDL_GetError());
            return;
        }

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (m_renderer == NULL)
        {
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL renderer: %s", SDL_GetError());
            return;
        }

        std::cout << "SDL Renderer from Window: " << m_renderer << std::endl;
        Renderer::SetSDLRenderer(m_renderer);
    }

    Window::~Window()
    {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
}
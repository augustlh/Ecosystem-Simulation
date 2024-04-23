#include "Window.h"
#include "Renderer.h"

namespace Ecosim
{
    Window::Window(const char *title, uint width, uint height)
    {
        Create(title, width, height);
    }

    Window::~Window()
    {
        if (m_window != nullptr)
        {
            SDL_DestroyWindow(m_window);
            SDL_Quit();
        }
    }

    void Window::Create(const char *title, uint width, uint height)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to initialize: %s", SDL_GetError());
            return;
        }

        m_window = SDL_CreateWindow(title, width, height, 0);
        if (m_window == NULL)
        {
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL window: %s", SDL_GetError());
            return;
        }

        Renderer::SetupSDLRenderSurface(m_window);
    }

    Window &Window::operator=(Window &&other)
    {
        if (this != &other)
        {
            this->m_window = other.m_window;
            other.m_window = nullptr;
        }
        return *this;
    }
}
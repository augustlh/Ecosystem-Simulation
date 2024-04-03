#include "Window.h"
#include "Renderer.h"

namespace Ecosim
{
    Window::Window(const char *title, unsigned int width, unsigned int height)
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

    void Window::Create(const char *title, unsigned int width, unsigned int height)
    {
        m_width = width;
        m_height = height;

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to initialize: %s", SDL_GetError());
            return;
        }

        m_window = SDL_CreateWindow(title, m_width, m_height, 0);
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
            this->m_width = other.m_width;
            this->m_height = other.m_height;

            other.m_window = nullptr;
            other.m_width = 0;
            other.m_height = 0;
        }

        return *this;
    }
}
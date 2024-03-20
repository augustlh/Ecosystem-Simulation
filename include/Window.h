#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

namespace Ecosim
{
    class Window
    {
    private:
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;

        unsigned int m_width, m_height;

    public:
        Window(const char *title, unsigned int width, unsigned int height);
        ~Window();

        const unsigned int &width, &height;
    };
}

#endif /* WINDOW_H */
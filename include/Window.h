#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>

namespace Ecosim
{
    class Window
    {
    private:
        SDL_Window *m_window;
        unsigned int m_width, m_height;

    public:
        Window() = default;
        Window(const char *title, unsigned int width, unsigned int height);
        ~Window();

        void Create(const char *title, unsigned int width, unsigned int height);

        Window &operator=(Window &&other);
    };
}

#endif /* WINDOW_H */
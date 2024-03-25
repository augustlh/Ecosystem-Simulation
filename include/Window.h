#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

namespace Ecosim
{
    class Window
    {
        // friend class Window;

    private:
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;

        unsigned int m_width, m_height;

    public:
        const unsigned int &width, &height;

    public:
        Window();
        Window(const char *title, unsigned int width, unsigned int height);
        ~Window();

        void Create(const char *title, unsigned int width, unsigned int height);

        Window &operator=(Window &&other);
    };
}

#endif /* WINDOW_H */
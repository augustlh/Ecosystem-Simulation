#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

class Window
{
private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Surface *m_surface;

    unsigned int m_width, m_height;

public:
    Window(const char *title, unsigned int width, unsigned int height);
    ~Window();

    void Renderer();

    const unsigned int &width, &height;
};

#endif /* WINDOW_H */
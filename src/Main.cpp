#include <iostream>
#include "Window.h"

int main(int argc, char *args[])
{
    Window win("EcoSim", 400, 400);

    SDL_Event windowEvent;
    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
            {
                break;
            }
        }
        win.Renderer();
    }

    return 0;
}

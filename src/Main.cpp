#include <iostream>

#include "Window.h"
#include "Renderer.h"

int main(int argc, char *args[])
{
    Ecosim::Window win("EcoSim", 400, 400);

    Ecosim::Vector2i circleCenter;
    int circleRadius = 100;
    Ecosim::Color circleColor = {.r = 211, .g = 137, .b = 67, .a = 255};

    Ecosim::Color backgroundColor = {.r = 43, .g = 38, .b = 66, .a = 255};

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

        Ecosim::Renderer::Background(backgroundColor);
        Ecosim::Renderer::Circle(circleCenter, circleRadius, circleColor);
        Ecosim::Renderer::RenderFrame();
    }

    return 0;
}

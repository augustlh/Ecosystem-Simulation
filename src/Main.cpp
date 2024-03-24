#include <iostream>
#include <vector>
#include <memory>

#include "Window.h"
#include "Renderer.h"
#include "Interfaces.h"
#include "Map.h"

class GreenRenderable : public Ecosim::Renderable, public Ecosim::Simulatable
{
private:
    Ecosim::Color m_color;
    Ecosim::Vector2<int> m_center;
    Ecosim::Vector2<int> m_velocity;
    int m_radius;

public:
    GreenRenderable() : m_color({.r = 0, .g = 200, .b = 80, .a = 255}), m_center(120, 200), m_velocity(0), m_radius(40) {}

    void Draw() override
    {
        Ecosim::Vector2<int> direction(m_velocity.x * m_radius + 10, m_velocity.y * m_radius + 10);
        Ecosim::Renderer::Line(m_center, m_center + direction, m_color);
        Ecosim::Renderer::Circle(m_center, m_radius, m_color);
    }

    void Step() override
    {
        m_velocity.x = (std::rand() % 10) - 5;
        m_velocity.y = (std::rand() % 10) - 5;

        m_center.x += m_velocity.x;
        m_center.y += m_velocity.y;
    }
};

class RedRenderable : public Ecosim::Renderable, public Ecosim::Simulatable
{
private:
    Ecosim::Color m_color;
    Ecosim::Vector2<int> m_center;
    Ecosim::Vector2<int> m_velocity;
    int m_radius;

public:
    RedRenderable() : m_color({.r = 200, .g = 80, .b = 0, .a = 255}), m_center(300, 200), m_velocity(0), m_radius(60) {}

    void Draw() override
    {
        Ecosim::Vector2<int> direction(m_velocity.x * m_radius + 10, m_velocity.y * m_radius + 10);
        Ecosim::Renderer::Line(m_center, m_center + direction, m_color);
        Ecosim::Renderer::Circle(m_center, m_radius, m_color);
    }

    void Step() override
    {
        m_velocity.x = (std::rand() % 10) - 5;
        m_velocity.y = (std::rand() % 10) - 5;

        m_center.x += m_velocity.x;
        m_center.y += m_velocity.y;
    }
};

int main(int argc, char *args[])
{
    Ecosim::Window win("Ecosim", 800, 800);

    Ecosim::Color backgroundColor = {.r = 43, .g = 38, .b = 66, .a = 255};

    std::shared_ptr<GreenRenderable> obj1 = std::make_shared<GreenRenderable>();
    std::shared_ptr<RedRenderable> obj2 = std::make_shared<RedRenderable>();

    std::vector<std::shared_ptr<Ecosim::Renderable>> renderables;
    renderables.push_back(obj1);
    renderables.push_back(obj2);

    std::vector<std::shared_ptr<Ecosim::Simulatable>> simulatables;
    simulatables.push_back(obj1);
    simulatables.push_back(obj2);

    SDL_Event sdlEvent;
    bool shouldClose = false;

    Ecosim::Map map;

    while (!shouldClose)
    {
        while (SDL_PollEvent(&sdlEvent))
        {
            if (sdlEvent.type == SDL_QUIT)
                shouldClose = true;
        }

        for (const auto &simulatable : simulatables)
            simulatable->Step(/*delta time*/);

        // Ecosim::Renderer::Background(backgroundColor);
        map.Render();

        for (const auto &renderable : renderables)
            renderable->Draw();

        Ecosim::Renderer::RenderFrame();
    }

    return 0;
}

// unsigned int startTick = SDL_GetTicks();
// unsigned int numIterations = 100;
// size_t frameCount = 0;

// ++frameCount;
// if (frameCount % numIterations == 0)
// {
//     unsigned int nowTick = SDL_GetTicks();
//     std::cout << "FPS over " << numIterations << " frames: " << numIterations / ((nowTick - startTick) / 1000.0) << std::endl;
//     startTick = nowTick;
//     frameCount = 0;
// }

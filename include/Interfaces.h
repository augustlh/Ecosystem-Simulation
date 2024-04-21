#ifndef INTERFACES_H
#define INTERFACES_H

namespace Ecosim
{
    class Renderable
    {
    public:
        virtual void Draw() = 0;
        virtual ~Renderable() {}
    };

    class Simulatable
    {
    public:
        virtual void Step(double deltaTime) = 0;
        virtual ~Simulatable() {}
    };
};

#endif /* INTERFACES_H */
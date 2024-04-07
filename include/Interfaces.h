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
        virtual void Step() = 0;
        virtual ~Simulatable() {}
    };

    class Collidable
    {
    public:
        virtual bool Collides(Collidable *other);
        virtual ~Collidable(){};
    };
};

#endif /* INTERFACES_H */
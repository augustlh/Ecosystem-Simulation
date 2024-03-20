#ifndef VECTOR2_H
#define VECTOR2_H

namespace Ecosim
{
    class Vector2
    {
    public:
        float x, y;

        Vector2();
        Vector2(float _n);
        Vector2(float _x, float _y);
    };

    class Vector2i
    {
    public:
        int x, y;

        Vector2i();
        Vector2i(int _n);
        Vector2i(int _x, int _y);

        Vector2i operator+(const Vector2i &other) const;
    };
}

#endif
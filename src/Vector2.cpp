#include "Vector2.h"

namespace Ecosim
{
    template <typename T>
    Vector2<T>::Vector2() : x(0), y(0) {}

    template <typename T>
    Vector2<T>::Vector2(T _n) : x(_n), y(_n) {}

    template <typename T>
    Vector2<T>::Vector2(T _x, T _y) : x(_x), y(_y) {}

    template <typename T>
    Vector2<T> Vector2<T>::operator+(const Vector2<T> &other) const { return Vector2<T>(x + other.x, y + other.y); }
}

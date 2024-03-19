#include "Vector2.h"

namespace Ecosim
{
    Vector2::Vector2() : x(0.0f), y(0.0f) {}
    Vector2::Vector2(float _n) : x(_n), y(_n) {}
    Vector2::Vector2(float _x, float _y) : x(_x), y(_y) {}

    Vector2i::Vector2i() : x(0), y(0) {}
    Vector2i::Vector2i(int _n) : x(_n), y(_n) {}
    Vector2i::Vector2i(int _x, int _y) : x(_x), y(_y) {}
}

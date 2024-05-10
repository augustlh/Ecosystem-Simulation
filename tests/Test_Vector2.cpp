#include "UnitTests.h"
#include "Vector2.h"
#include <iostream>
#include <cmath>

using Vec2 = Ecosim::Vector2<float>;
using Vec2i = Ecosim::Vector2<int>;

void Vector_Int_Arithmetics()
{
    Vec2i a(10, 20), b(-5, -15);
    Vec2i c = a + b;

    FAIL_IF_FALSE(c.x == 5 && c.y == 5, "Addition");
    c = a - b;
    FAIL_IF_FALSE(c.x == 15 && c.y == 35, "Subtraction");

    c *= 2;
    FAIL_IF_FALSE(c.x == 30 && c.y == 70, "Scalar multiplication");
    c /= 5;
    FAIL_IF_FALSE(c.x == 6 && c.y == 14, "Scalar division");
}

bool Equals(float a, float b)
{
    return std::abs(a - b) < 1e-8;
}

void Vector_Float_Arithmetics()
{
    Vec2 a(10, 20), b(-5, -15);
    Vec2 c = a + b;

    FAIL_IF_FALSE(Equals(c.x, 5.0f) && Equals(c.y, 5.0f), "Addition");
    c = a - b;
    FAIL_IF_FALSE(Equals(c.x, 15.0f) && Equals(c.y, 35.0f), "Subtraction");

    c *= 2.5;
    FAIL_IF_FALSE(Equals(c.x, 37.5f) && Equals(c.y, 87.5f), "Scalar multiplication");
    c /= 5;
    FAIL_IF_FALSE(Equals(c.x, 7.5f) && Equals(c.y, 17.5f), "Scalar division");
}

int main(void)
{
    TEST_FUNCTION(Vector_Int_Arithmetics);
    TEST_FUNCTION(Vector_Float_Arithmetics);
    return 0;
}
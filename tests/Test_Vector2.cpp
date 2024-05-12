#include "UnitTests.h"
#include "Vector2.h"
#include <iostream>
#include <cmath>

using Vec2 = Ecosim::Vector2<float>;
using Vec2i = Ecosim::Vector2<int>;

bool Equals(float a, float b)
{
    return std::abs(a - b) < 1e-8;
}

void Vector_Constructor()
{
    Vec2 v1;
    FAIL_IF_FALSE(Equals(v1.x, 0) && Equals(v1.y, 0), "Default constructor");

    Vec2 v2(1.5f);
    FAIL_IF_FALSE(Equals(v2.x, 1.5f) && Equals(v2.y, 1.5f), "Single value constructor");

    Vec2 v3(2.5f, 3.5f);
    FAIL_IF_FALSE(Equals(v3.x, 2.5f) && Equals(v3.y, 3.5f), "Two-value constructor");
}

void Vector_Conversion()
{
    Vec2i v1(2, 3);
    Vec2 v2 = v1.Convert<float>();

    FAIL_IF_FALSE(Equals(v2.x, 2.0f) && Equals(v2.y, 3.0f), "Conversion from int to float");
}

void Dot_Product()
{
    Vec2 v1(2, 3), v2(4, 5);
    float dot = v1.Dot(v2);

    FAIL_IF_FALSE(Equals(dot, 23.0f), "Dot product calculation");
}

void Magnitude()
{
    Vec2 v(3, 4);
    float mag = v.Magnitude();

    FAIL_IF_FALSE(Equals(mag, 5.0f), "Magnitude calculation");
}

void Vector_Copy()
{
    Vec2 v1(2, 3);
    Vec2 v2 = v1.Copy();

    FAIL_IF_FALSE(v2.x == 2 && v2.y == 3, "Copy method");
}

void Mathematical_Operators()
{
    Vec2 v1(1, 2), v2(3, 4);
    Vec2 v3 = v1 + v2;
    Vec2 v4 = v1 - v2;
    Vec2 v5 = v1 * 2.0f;
    Vec2 v6 = v2 / 2.0f;

    FAIL_IF_FALSE(Equals(v3.x, 4) && Equals(v3.y, 6), "Addition operator");
    FAIL_IF_FALSE(Equals(v4.x, -2) && Equals(v4.y, -2), "Subtraction operator");
    FAIL_IF_FALSE(Equals(v5.x, 2) && Equals(v5.y, 4), "Multiplication operator");
    FAIL_IF_FALSE(Equals(v6.x, 1.5f) && Equals(v6.y, 2.0f), "Division operator");
}

int main(void)
{
    std::cout << "Testing Vector2<T>\n";
    TEST_FUNCTION(Vector_Constructor);
    TEST_FUNCTION(Vector_Conversion);
    TEST_FUNCTION(Dot_Product);
    TEST_FUNCTION(Magnitude);
    TEST_FUNCTION(Vector_Copy);
    TEST_FUNCTION(Mathematical_Operators);
    return 0;
}
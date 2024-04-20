#ifndef VECTOR2_H
#define VECTOR2_H

#include <iostream>
#include <cmath>

namespace Ecosim
{
    /**
     * @brief A template 2D vector class with components T
     * @tparam T The datatype for each coordinate (float, int, double, ...)
     */
    template <typename T>
    class Vector2
    {
    public:
        /**
         * @brief The x and y coordinates of the vector
         */
        T x, y;

        /**
         * @brief Initializes both coordinates to zero
         */
        Vector2() = default;

        /**
         * @brief Initializes both coordinates to a single value given by `_n`
         * @param _n The value to set for both coordinates
         */
        Vector2(T _n) : x(_n), y(_n) {}

        /**
         * @brief Initializes the x and y coordinates with the given values
         * @param _x The value to set for the `x` coordinate
         * @param _y The value to set for the `y` coordinate
         */
        Vector2(T _x, T _y) : x(_x), y(_y) {}

        // Vector2<int> Round() { return Vector2<int>(static_cast<int>(x), static_cast<int>(y)); }

        template <typename R>
        Vector2<R> Convert() const { return Vector2<R>(static_cast<R>(x), static_cast<R>(y)); }

        float Magnitude() const { return std::sqrt(x * x + y * y); }

        /**
         * @brief Vector-vector addition, returning a new Vector2<T> object
         * @param other The vector to add to this vector
         * @return The elementwise sum of the two vectors
         */
        Vector2<T> operator+(const Vector2<T> &other) const { return Vector2<T>(x + other.x, y + other.y); }

        Vector2<T> operator-(const Vector2<T> &other) const { return Vector2<T>(x - other.x, y - other.y); }

        Vector2<T> operator*(T scalar) const { return Vector2<T>(x * scalar, y * scalar); }
        Vector2<T> operator*(Vector2<T> &vec) const { return Vector2<T>(x * vec.x, y * vec.y); }

        Vector2<T> operator/(T scalar) const { return Vector2<T>(x / scalar, y / scalar); }
        Vector2<T> operator/(Vector2<T> &vec) const { return Vector2<T>(x / vec.x, y / vec.y); }

        Vector2<T> &operator+=(const Vector2<T> &other)
        {
            this->x += other.x;
            this->y += other.y;
            return *this;
        }

        Vector2<T> &operator-=(const Vector2<T> &other)
        {
            this->x -= other.x;
            this->y -= other.y;
            return *this;
        }

        Vector2<T> &operator*=(T scalar)
        {
            this->x *= scalar;
            this->y *= scalar;
            return *this;
        }

        Vector2<T> &operator/=(T scalar)
        {
            this->x /= scalar;
            this->y /= scalar;
            return *this;
        }
    };
}

#endif
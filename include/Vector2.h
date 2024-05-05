#ifndef VECTOR2_H
#define VECTOR2_H

#include <iostream>
#include <cmath>

namespace Ecosim
{
    /// @brief A 2-dimensional vector class
    /// @tparam T The datatype of the x and y-coordinate, such as float or int
    template <typename T>
    class Vector2
    {
    public:
        /// @brief The x and y coordinates of the vector
        T x, y;

        /// @brief Initializes both coordinates to zero
        Vector2() = default;

        /// @brief Initializes both coordinates to a single value given by `_n`
        /// @param _n The value to set for both coordinates
        Vector2(T _n) : x(_n), y(_n) {}

        /// @brief Initializes the x and y coordinates with the given values
        /// @param _x The value to set for the `x` coordinate
        /// @param _y The value to set for the `y` coordinate
        Vector2(T _x, T _y) : x(_x), y(_y) {}

        /// @brief Convert this vector-object to a `Vector<R>`-object
        /// @tparam R The datatype of the resulting vector
        /// @return A copy of this vector with its coords converted to `R`
        template <typename R>
        Vector2<R> Convert() const { return Vector2<R>(static_cast<R>(x), static_cast<R>(y)); }

        /// @return The magnitude of the vector
        float Magnitude() const { return std::sqrt(x * x + y * y); }

        /// @return The square magnitude of the vector
        float SquareMagnitude() const { return (x * x + y * y); }

        /// @brief Vector-vector addition, returning a new `Vector2<T>`-object
        /// @param other The vector to add to this vector
        /// @return The elementwise sum of the two vectors
        Vector2<T> operator+(const Vector2<T> &other) const { return Vector2<T>(x + other.x, y + other.y); }

        /// @brief Vector-vector subtraction, returning a new `Vector2<T>`-object
        /// @param other The vector to subtract from this vector
        /// @return The elementwise result of the two vectors
        Vector2<T> operator-(const Vector2<T> &other) const { return Vector2<T>(x - other.x, y - other.y); }

        /// @brief Vector-scalar multiplication, returning a new `Vector2<T>`-object
        /// @param scalar The scalar
        /// @return The scaled vector
        Vector2<T> operator*(T scalar) const { return Vector2<T>(x * scalar, y * scalar); }

        /// @brief Vector-scalar division, returning a new `Vector2<T>`-object
        /// @param scalar The scalar
        /// @return The scaled vector
        Vector2<T> operator/(T scalar) const { return Vector2<T>(x / scalar, y / scalar); }

        /// @brief Vector-vector addition, modifing this vector
        /// @param other The vector to add to this vector
        /// @return A reference to this vector
        Vector2<T> &operator+=(const Vector2<T> &other)
        {
            this->x += other.x;
            this->y += other.y;
            return *this;
        }

        /// @brief Vector-vector subtraction, modifing this vector
        /// @param other The vector to subtract from this vector
        /// @return A reference to this vector
        Vector2<T> &operator-=(const Vector2<T> &other)
        {
            this->x -= other.x;
            this->y -= other.y;
            return *this;
        }

        /// @brief Vector-scalar multiplication, modifing this vector
        /// @param scalar The scalar
        /// @return A reference to this vector
        Vector2<T> &operator*=(T scalar)
        {
            this->x *= scalar;
            this->y *= scalar;
            return *this;
        }

        /// @brief Vector-scalar division, modifing this vector
        /// @param scalar The scalar
        /// @return A reference to this vector
        Vector2<T> &operator/=(T scalar)
        {
            this->x /= scalar;
            this->y /= scalar;
            return *this;
        }
    };
}

#endif
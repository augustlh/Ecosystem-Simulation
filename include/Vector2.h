#ifndef VECTOR2_H
#define VECTOR2_H

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
        Vector2() : x(0), y(0) {}

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

        /**
         * @brief Vector-vector addition, returning a new Vector2<T> object
         * @param other The vector to add to this vector
         * @return The elementwise sum of the two vectors
         */
        Vector2<T> &operator+(const Vector2<T> &other) const { return Vector2<T>(x + other.x, y + other.y); }
    };

    // template class Ecosim::Vector2<int>;
    // template class Ecosim::Vector2<float>;
}

#endif
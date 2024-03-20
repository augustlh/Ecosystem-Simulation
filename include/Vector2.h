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
        Vector2();

        /**
         * @brief Initializes both coordinates to a single value given by `_n`
         * @param _n The value to set for both coordinates
         */
        Vector2(T _n);

        /**
         * @brief Initializes the x and y coordinates with the given values
         * @param _x The value to set for the `x` coordinate
         * @param _y The value to set for the `y` coordinate
         */
        Vector2(T _x, T _y);

        /**
         * @brief Vector-vector addition, returning a new Vector2<T> object
         * @param other The vector to add to this vector
         * @return The elementwise sum of the two vectors
         */
        Vector2 operator+(const Vector2 &other) const;
    };
}

template class Ecosim::Vector2<int>;
template class Ecosim::Vector2<float>;

#endif
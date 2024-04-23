#ifndef INTERFACES_H
#define INTERFACES_H

namespace Ecosim
{
    /// @brief An interface for all objects that can be rendered to the screen
    class Renderable
    {
    public:
        /// @brief Draws the object to the screen
        virtual void Draw() = 0;

        /// @brief A destructor to insure sub-classes are properly destructed, if freed through a `Renderable`-pointer
        virtual ~Renderable() {}
    };

    /// @brief An interface for all objects that can be simulated
    class Simulatable
    {
    public:
        /// @brief Updates the object
        /// @param deltaTime The time in seconds since the last frame
        virtual void Step(double deltaTime) = 0;

        /// @brief A destructor to insure sub-classes are properly destructed, if freed through a `Simulatable`-pointer
        virtual ~Simulatable() {}
    };
};

#endif /* INTERFACES_H */
#ifndef INTERFACES_H
#define INTERFACES_H

#include "Vector2.h"

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

    /// @brief Enum that defines types of collidable objects.
    enum CollidableType
    {
        AGENT,
        FOOD
    };

    /// @brief An interface for all objects that can collide. Theese objects are seen as Eatables, meaning a Collidable object can be eaten
    class Collidable
    {
    public:
        /// @brief A destructor to insure sub-classes are properly destructed, if freed through a `Collidable`-pointer
        virtual ~Collidable() = default;

        /// @brief Returns the id of this object
        virtual int GetId() = 0;

        /// @brief Returns the radius of this object
        virtual int getRadius() = 0;

        /// @brief Checks if this object collides with another object
        virtual bool Collides(std::shared_ptr<Collidable> &other) = 0;

        /// @brief Handles the collision with another object
        virtual void handleCollision(std::shared_ptr<Collidable> &other) = 0;

        /// @brief Returns the type of this object
        virtual CollidableType getType() = 0;

        /// @brief Returns the position of this object
        virtual Vector2<float> getPosition() = 0;

        /// @brief Returns the energy of this object. This is a pure virtual function from the Eatable interface
        virtual float getEnergy() = 0;

        /// @brief Respawn the object at a random location
        virtual void OnEaten() = 0;
    };
};

#endif /* INTERFACES_H */
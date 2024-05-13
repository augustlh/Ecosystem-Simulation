#ifndef WINDOW_H
#define WINDOW_H

#include "Ecosim.h"

namespace Ecosim
{
    /// @brief An SDL_Window wrapper
    class ECOSIM_API Window
    {
    private:
        /// @brief A pointer to the SDL_Window
        SDL_Window *m_window;

    public:
        /// @brief A default `Window`-constuctor, which does not create a `SDL_Window`
        Window() = default;

        /// @brief Creates a `Window`-object and calls the `Create`-function
        /// @param title The title of the window
        /// @param width The width of the window
        /// @param height The height of the window
        Window(const char *title, uint width, uint height);

        /// @brief Destroyes this window and quits SDL
        ~Window();

        /// @brief Create the `SDL_Window`
        /// @param title The title of the window
        /// @param width The width of the window
        /// @param height The height of the window
        void Create(const char *title, uint width, uint height);

        /// @brief Overloading the assignment operator
        /// @param other The other window
        /// @return A reference to this window
        Window &operator=(Window &&other);
    };
}

#endif /* WINDOW_H */
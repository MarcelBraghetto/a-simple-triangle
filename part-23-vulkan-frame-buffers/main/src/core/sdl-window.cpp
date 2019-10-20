#include "sdl-window.hpp"

using ast::SDLWindow;

struct SDLWindow::Internal
{
    SDL_Window* window;

    Internal(const uint32_t& windowFlags) : window(ast::sdl::createWindow(windowFlags)) {}

    ~Internal()
    {
        SDL_DestroyWindow(window);
    }
};

SDLWindow::SDLWindow(const uint32_t& windowFlags) : internal(ast::make_internal_ptr<Internal>(windowFlags)) {}

SDL_Window* SDLWindow::getWindow() const
{
    return internal->window;
}

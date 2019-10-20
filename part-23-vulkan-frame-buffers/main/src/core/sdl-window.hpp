#pragma once

#include "internal-ptr.hpp"
#include "sdl-wrapper.hpp"

namespace ast
{
    struct SDLWindow
    {
        SDLWindow(const uint32_t& windowFlags);

        SDL_Window* getWindow() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

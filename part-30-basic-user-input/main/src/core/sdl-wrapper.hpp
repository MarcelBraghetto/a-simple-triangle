#pragma once

#include <SDL.h>
#ifndef __EMSCRIPTEN__
#include <SDL_vulkan.h>
#endif
#include "window-size.hpp"

namespace ast::sdl
{
    ast::WindowSize getInitialWindowSize();

    ast::WindowSize getWindowSize(SDL_Window* window);

    SDL_Window* createWindow(const uint32_t& windowFlags);
} // namespace ast::sdl

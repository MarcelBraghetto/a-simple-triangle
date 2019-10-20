#pragma once

#include <SDL.h>
#ifndef __EMSCRIPTEN__
#include <SDL_vulkan.h>
#endif
#include <utility>

namespace ast::sdl
{
    std::pair<uint32_t, uint32_t> getDisplaySize();

    SDL_Window* createWindow(const uint32_t& windowFlags);
} // namespace ast::sdl

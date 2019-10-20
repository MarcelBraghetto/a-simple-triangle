#pragma once

#include "internal-ptr.hpp"
#include "sdl-wrapper.hpp"

namespace ast
{
    struct Bitmap
    {
        Bitmap(SDL_Surface* surface);

        uint16_t getWidth() const;

        uint16_t getHeight() const;

        void* getPixelData() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

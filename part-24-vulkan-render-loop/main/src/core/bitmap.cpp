#include "bitmap.hpp"

using ast::Bitmap;

struct Bitmap::Internal
{
    SDL_Surface* surface;

    Internal(SDL_Surface* surface) : surface(surface) {}

    ~Internal()
    {
        SDL_FreeSurface(surface);
    }
};

Bitmap::Bitmap(SDL_Surface* surface) : internal(ast::make_internal_ptr<Internal>(surface)) {}

uint16_t Bitmap::getWidth() const
{
    return static_cast<uint16_t>(internal->surface->w);
}

uint16_t Bitmap::getHeight() const
{
    return static_cast<uint16_t>(internal->surface->h);
}

void* Bitmap::getPixelData() const
{
    return internal->surface->pixels;
}

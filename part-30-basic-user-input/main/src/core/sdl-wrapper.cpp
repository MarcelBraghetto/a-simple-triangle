#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "platform.hpp"
#include "sdl-wrapper.hpp"

namespace
{
    bool shouldDisplayFullScreen()
    {
        switch (ast::getCurrentPlatform())
        {
            case ast::Platform::ios:
            case ast::Platform::android:
                return true;

            default:
                return false;
        }
    }

#ifdef __EMSCRIPTEN__
    ast::WindowSize getEmscriptenCanvasSize()
    {
        // For Emscripten targets we will invoke some Javascript
        // to find out the dimensions of the canvas in the HTML
        // document. Note that the 'width' and 'height' attributes
        // need to be set on the <canvas /> HTML element, like so:
        // <canvas id="canvas" width="600", height="360"></canvas>
        uint32_t width{static_cast<uint32_t>(EM_ASM_INT({
            return document.getElementById('canvas').width;
		}))};

        uint32_t height{static_cast<uint32_t>(EM_ASM_INT({
            return document.getElementById('canvas').height;
		}))};

        return ast::WindowSize{width, height};
    }
#endif
} // namespace

ast::WindowSize ast::sdl::getWindowSize(SDL_Window* window)
{
#ifdef __EMSCRIPTEN__
    return ::getEmscriptenCanvasSize();
#else
    int width{0};
    int height{0};
    SDL_GetWindowSize(window, &width, &height);
    return ast::WindowSize{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
#endif
}

ast::WindowSize ast::sdl::getInitialWindowSize()
{
#ifdef __EMSCRIPTEN__
    return ::getEmscriptenCanvasSize();
#else
    const ast::Platform platform{ast::getCurrentPlatform()};

    if (platform == ast::Platform::ios || platform == ast::Platform::android)
    {
        // For mobile platforms we will fetch the full screen size.
        SDL_DisplayMode displayMode;
        SDL_GetDesktopDisplayMode(0, &displayMode);
        return ast::WindowSize{static_cast<uint32_t>(displayMode.w), static_cast<uint32_t>(displayMode.h)};
    }

    // For other platforms we'll just show a fixed size window.
    return ast::WindowSize{640, 480};
#endif
}

SDL_Window* ast::sdl::createWindow(const uint32_t& windowFlags)
{
    ast::WindowSize windowSize{ast::sdl::getInitialWindowSize()};

    SDL_Window* window{SDL_CreateWindow(
        "A Simple Triangle",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        windowSize.width, windowSize.height,
        windowFlags)};

    if (::shouldDisplayFullScreen())
    {
        SDL_SetWindowFullscreen(window, SDL_TRUE);
    }

    return window;
}

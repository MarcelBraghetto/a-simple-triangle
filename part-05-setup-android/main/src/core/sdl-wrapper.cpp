#include "sdl-wrapper.hpp"
#include "platform.hpp"

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
} // namespace

std::pair<uint32_t, uint32_t> ast::sdl::getDisplaySize()
{
    uint32_t displayWidth{0};
    uint32_t displayHeight{0};

    switch (ast::getCurrentPlatform())
    {
        case ast::Platform::ios:
        case ast::Platform::android:
        {
            // For mobile platforms we will fetch the full screen size.
            SDL_DisplayMode displayMode;
            SDL_GetDesktopDisplayMode(0, &displayMode);
            displayWidth = static_cast<uint32_t>(displayMode.w);
            displayHeight = static_cast<uint32_t>(displayMode.h);
            break;
        }

        default:
        {
            // For other platforms we'll just show a fixed size window.
            displayWidth = 640;
            displayHeight = 480;
            break;
        }
    }

    return std::make_pair(displayWidth, displayHeight);
}

SDL_Window* ast::sdl::createWindow(const uint32_t& windowFlags)
{
    std::pair<uint32_t, uint32_t> displaySize{ast::sdl::getDisplaySize()};

    SDL_Window* window{SDL_CreateWindow(
        "A Simple Triangle",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        displaySize.first, displaySize.second,
        windowFlags)};

    if (::shouldDisplayFullScreen())
    {
        SDL_SetWindowFullscreen(window, SDL_TRUE);
    }

    return window;
}

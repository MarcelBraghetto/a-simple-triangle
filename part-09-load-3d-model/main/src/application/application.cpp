#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "../core/sdl-wrapper.hpp"
#include "application.hpp"

using ast::Application;

namespace
{
#ifdef EMSCRIPTEN
    void emscriptenMainLoop(ast::Application* application)
    {
        application->runMainLoop();
    }
#endif
} // namespace

void Application::startApplication()
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg((em_arg_callback_func)::emscriptenMainLoop, this, 0, 1);
#else
    while (runMainLoop())
    {
        // Just waiting for the main loop to end.
    }
#endif
}

bool Application::runMainLoop()
{
    SDL_Event event;

    // Each loop we will process any events that are waiting for us.
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                return false;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return false;
                }
                break;
            default:
                break;
        }
    }

    // Perform our rendering for this frame.
    render();

    return true;
}

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

struct Application::Internal
{
    const float performanceFrequency;
    uint64_t currentTime;
    uint64_t previousTime;

    Internal() : performanceFrequency(static_cast<float>(SDL_GetPerformanceFrequency())),
                 currentTime(SDL_GetPerformanceCounter()),
                 previousTime(currentTime) {}

    float timeStep()
    {
        previousTime = currentTime;
        currentTime = SDL_GetPerformanceCounter();

        float elapsed{(currentTime - previousTime) * 1000.0f};
        return (elapsed / performanceFrequency) * 0.001f;
    }
};

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

    // Perform our updating for this frame.
    update(internal->timeStep());

    // Perform our rendering for this frame.
    render();

    return true;
}

Application::Application() : internal(ast::make_internal_ptr<Internal>()) {}

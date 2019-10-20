#include "core/graphics-wrapper.hpp"
#include "core/sdl-wrapper.hpp"
#include <iostream>

void render(SDL_Window* window, const SDL_GLContext& context)
{
    SDL_GL_MakeCurrent(window, context);

    glClearColor(0.3f, 0.7f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SDL_GL_SwapWindow(window);
}

bool runMainLoop(SDL_Window* window, const SDL_GLContext& context)
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
    render(window, context);

    return true;
}

void runApplication()
{
    std::pair<uint32_t, uint32_t> displaySize{ast::sdl::getDisplaySize()};

    std::cout << "Display size is: " << displaySize.first << " x " << displaySize.second << std::endl;

    // Create a new SDL window based on OpenGL.
    SDL_Window* window{ast::sdl::createWindow(SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI)};

    // Obtain an OpenGL context based on our window.
    SDL_GLContext context{SDL_GL_CreateContext(window)};

    // Setup some basic global OpenGL state.
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, displaySize.first, displaySize.second);

    while (runMainLoop(window, context))
    {
        // Just waiting for the main loop to end.
    }

    // Clean up after ourselves.
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
}

int main(int, char* [])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0)
    {
        std::cout << "Successfully initialised SDL!" << std::endl;

        runApplication();

        SDL_Quit();
    }
    else
    {
        std::cout << "Failed to initialise SDL!" << std::endl;
    }

    return 0;
}

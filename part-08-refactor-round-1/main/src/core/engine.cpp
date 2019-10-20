#include "engine.hpp"
#include "../application/application.hpp"
#include "../application/opengl/opengl-application.hpp"
#include "log.hpp"
#include "sdl-wrapper.hpp"
#include <stdexcept>
#include <string>

using ast::Engine;

struct Engine::Internal
{
    const std::string classLogTag;

    Internal() : classLogTag("ast::Engine::") {}

    void run()
    {
        static const std::string logTag{classLogTag + "run"};

        ast::log(logTag, "Starting engine ...");
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        ast::log(logTag, "SDL2 initialized successfully ...");
        resolveApplication()->startApplication();
    }

    std::unique_ptr<ast::Application> resolveApplication()
    {
        static const std::string logTag{classLogTag + "resolveApplication"};

        try
        {
            ast::log(logTag, "Creating OpenGL application ...");
            return std::make_unique<ast::OpenGLApplication>();
        }
        catch (const std::exception& error)
        {
            ast::log(logTag, "OpenGL application failed to initialize.", error);
        }

        throw std::runtime_error(logTag + " No applications can run in the current environment");
    }

    ~Internal()
    {
        SDL_Quit();
    }
};

Engine::Engine() : internal(ast::make_internal_ptr<Internal>()) {}

void Engine::run()
{
    internal->run();
}

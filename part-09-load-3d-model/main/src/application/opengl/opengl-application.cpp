#include "opengl-application.hpp"
#include "../../core/assets.hpp"
#include "../../core/graphics-wrapper.hpp"
#include "../../core/log.hpp"
#include "../../core/sdl-wrapper.hpp"
#include <string>

using ast::OpenGLApplication;

namespace
{
    SDL_GLContext createContext(SDL_Window* window)
    {
        static const std::string logTag{"ast::OpenGLApplication::createContext"};

        SDL_GLContext context{SDL_GL_CreateContext(window)};

#ifdef WIN32
        glewInit();
#endif

        int viewportWidth;
        int viewportHeight;
        SDL_GL_GetDrawableSize(window, &viewportWidth, &viewportHeight);
        ast::log(logTag, "Created OpenGL context with viewport size: " + std::to_string(viewportWidth) + " x " + std::to_string(viewportHeight));

        glClearDepthf(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        glViewport(0, 0, viewportWidth, viewportHeight);

        return context;
    }
} // namespace

struct OpenGLApplication::Internal
{
    SDL_Window* window;
    SDL_GLContext context;
    const ast::Mesh mesh;

    Internal() : window(ast::sdl::createWindow(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI)),
                 context(::createContext(window)),
                 mesh(ast::assets::loadOBJFile("assets/models/crate.obj"))
    {
        ast::log("CRATE!", "Crate has " + std::to_string(mesh.getVertices().size()) + " vertices and " + std::to_string(mesh.getIndices().size()) + " indices.");
    }

    void render()
    {
        SDL_GL_MakeCurrent(window, context);

        glClearColor(0.3f, 0.7f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }

    ~Internal()
    {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
    }
};

OpenGLApplication::OpenGLApplication() : internal(ast::make_internal_ptr<Internal>()) {}

void OpenGLApplication::render()
{
    internal->render();
}

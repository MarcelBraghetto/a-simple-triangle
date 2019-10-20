#include "opengl-application.hpp"
#include "../../core/graphics-wrapper.hpp"
#include "../../core/log.hpp"
#include "../../core/sdl-window.hpp"
#include "../../scene/scene-main.hpp"
#include "opengl-asset-manager.hpp"
#include "opengl-renderer.hpp"

using ast::OpenGLApplication;

namespace
{
    void updateViewport(SDL_Window* window)
    {
        static const std::string logTag{"ast::OpenGLApplication::updateViewport"};

        int viewportWidth;
        int viewportHeight;
        SDL_GL_GetDrawableSize(window, &viewportWidth, &viewportHeight);
        ast::log(logTag, "Created OpenGL context with viewport size: " + std::to_string(viewportWidth) + " x " + std::to_string(viewportHeight));

        glViewport(0, 0, viewportWidth, viewportHeight);
    }

    SDL_GLContext createContext(SDL_Window* window)
    {
        static const std::string logTag{"ast::OpenGLApplication::createContext"};

        SDL_GLContext context{SDL_GL_CreateContext(window)};

#ifdef WIN32
        glewInit();
#endif

        glClearDepthf(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);

        ::updateViewport(window);

        return context;
    }

    std::shared_ptr<ast::OpenGLAssetManager> createAssetManager()
    {
        return std::make_shared<ast::OpenGLAssetManager>(ast::OpenGLAssetManager());
    }

    ast::OpenGLRenderer createRenderer(std::shared_ptr<ast::OpenGLAssetManager> assetManager)
    {
        return ast::OpenGLRenderer(assetManager);
    }

    std::unique_ptr<ast::Scene> createMainScene(const ast::SDLWindow& window, ast::OpenGLAssetManager& assetManager)
    {
        std::unique_ptr<ast::Scene> scene{std::make_unique<ast::SceneMain>(ast::sdl::getWindowSize(window.getWindow()))};
        assetManager.loadAssetManifest(scene->getAssetManifest());
        scene->prepare();

        return scene;
    }
} // namespace

struct OpenGLApplication::Internal
{
    const ast::SDLWindow window;
    SDL_GLContext context;
    const std::shared_ptr<ast::OpenGLAssetManager> assetManager;
    ast::OpenGLRenderer renderer;
    std::unique_ptr<ast::Scene> scene;

    Internal() : window(ast::SDLWindow(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI)),
                 context(::createContext(window.getWindow())),
                 assetManager(::createAssetManager()),
                 renderer(::createRenderer(assetManager)) {}

    ast::Scene& getScene()
    {
        if (!scene)
        {
            scene = ::createMainScene(window, *assetManager);
        }

        return *scene;
    }

    void update(const float& delta)
    {
        getScene().update(delta);
    }

    void render()
    {
        SDL_GL_MakeCurrent(window.getWindow(), context);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        getScene().render(renderer);

        SDL_GL_SwapWindow(window.getWindow());
    }

    void onWindowResized()
    {
        getScene().onWindowResized(ast::sdl::getWindowSize(window.getWindow()));
        ::updateViewport(window.getWindow());
    }

    ~Internal()
    {
        SDL_GL_DeleteContext(context);
    }
};

OpenGLApplication::OpenGLApplication() : internal(ast::make_internal_ptr<Internal>()) {}

void OpenGLApplication::update(const float& delta)
{
    internal->update(delta);
}

void OpenGLApplication::render()
{
    internal->render();
}

void OpenGLApplication::onWindowResized()
{
    internal->onWindowResized();
}

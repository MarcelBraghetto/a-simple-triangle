#include "scene-main.hpp"
#include "../core/perspective-camera.hpp"
#include "../core/sdl-wrapper.hpp"
#include "../core/static-mesh-instance.hpp"
#include "player.hpp"

using ast::SceneMain;
using ast::assets::Pipeline;
using ast::assets::StaticMesh;
using ast::assets::Texture;

namespace
{
    ast::PerspectiveCamera createCamera(const ast::WindowSize& size)
    {
        return ast::PerspectiveCamera(static_cast<float>(size.width),
                                      static_cast<float>(size.height));
    }
} // namespace

struct SceneMain::Internal
{
    ast::PerspectiveCamera camera;
    std::vector<ast::StaticMeshInstance> staticMeshes;
    ast::Player player;
    const uint8_t* keyboardState;

    Internal(const ast::WindowSize& size)
        : camera(::createCamera(size)),
          player(ast::Player(glm::vec3{0.0f, 0.0f, 2.0f})),
          keyboardState(SDL_GetKeyboardState(nullptr)) {}

    ast::AssetManifest getAssetManifest()
    {
        return ast::AssetManifest{
            {Pipeline::Default},
            {StaticMesh::Crate, StaticMesh::Torus},
            {Texture::Crate, Texture::RedCrossHatch}};
    }

    void prepare()
    {
        staticMeshes.push_back(ast::StaticMeshInstance{
            StaticMesh::Crate,           // Mesh
            Texture::Crate,              // Texture
            glm::vec3{0.4f, 0.6f, 0.0f}, // Position
            glm::vec3{0.6f, 0.6f, 0.6f}, // Scale
            glm::vec3{0.0f, 0.4f, 0.9f}, // Rotation axis
            0.0f});                      // Initial rotation

        staticMeshes.push_back(ast::StaticMeshInstance{
            StaticMesh::Torus,            // Mesh
            Texture::RedCrossHatch,       // Texture
            glm::vec3{-0.6f, 0.4f, 0.0f}, // Position
            glm::vec3{0.4f, 0.4f, 0.4f},  // Scale
            glm::vec3{0.2f, 1.0f, 0.4f},  // Rotation axis
            0.0f});                       // Initial rotation

        staticMeshes.push_back(ast::StaticMeshInstance{
            StaticMesh::Crate,             // Mesh
            Texture::Crate,                // Texture
            glm::vec3{-0.5f, -0.5f, 0.0f}, // Position
            glm::vec3{0.7f, 0.3f, 0.3f},   // Scale
            glm::vec3{0.2f, 0.6f, 0.1f},   // Rotation axis
            90.0f});                       // Initial rotation

        staticMeshes.push_back(ast::StaticMeshInstance{
            StaticMesh::Torus,            // Mesh
            Texture::RedCrossHatch,       // Texture
            glm::vec3{0.6f, -0.4f, 0.0f}, // Position
            glm::vec3{0.4f, 0.4f, 0.4f},  // Scale
            glm::vec3{0.6f, 0.3f, 0.1f},  // Rotation axis
            50.0f});                      // Initial rotation
    }

    void processInput(const float& delta)
    {
        if (keyboardState[SDL_SCANCODE_UP])
        {
            player.moveForward(delta);
        }

        if (keyboardState[SDL_SCANCODE_DOWN])
        {
            player.moveBackward(delta);
        }

        if (keyboardState[SDL_SCANCODE_A])
        {
            player.moveUp(delta);
        }

        if (keyboardState[SDL_SCANCODE_Z])
        {
            player.moveDown(delta);
        }

        if (keyboardState[SDL_SCANCODE_LEFT])
        {
            player.turnLeft(delta);
        }

        if (keyboardState[SDL_SCANCODE_RIGHT])
        {
            player.turnRight(delta);
        }
    }

    void update(const float& delta)
    {
        processInput(delta);

        camera.configure(player.getPosition(), player.getDirection());

        const glm::mat4 cameraMatrix{camera.getProjectionMatrix() * camera.getViewMatrix()};

        for (auto& staticMesh : staticMeshes)
        {
            staticMesh.rotateBy(delta * 45.0f);
            staticMesh.update(cameraMatrix);
        }
    }

    void render(ast::Renderer& renderer)
    {
        renderer.render(Pipeline::Default, staticMeshes);
    }

    void onWindowResized(const ast::WindowSize& size)
    {
        camera = ::createCamera(size);
    }
};

SceneMain::SceneMain(const ast::WindowSize& size)
    : internal(ast::make_internal_ptr<Internal>(size)) {}

ast::AssetManifest SceneMain::getAssetManifest()
{
    return internal->getAssetManifest();
}

void SceneMain::prepare()
{
    internal->prepare();
}

void SceneMain::update(const float& delta)
{
    internal->update(delta);
}

void SceneMain::render(ast::Renderer& renderer)
{
    internal->render(renderer);
}

void SceneMain::onWindowResized(const ast::WindowSize& size)
{
    internal->onWindowResized(size);
}

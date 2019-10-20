#include "scene-main.hpp"
#include "../core/perspective-camera.hpp"
#include "../core/static-mesh-instance.hpp"

using ast::SceneMain;
using ast::assets::Pipeline;
using ast::assets::StaticMesh;
using ast::assets::Texture;

namespace
{
    ast::PerspectiveCamera createCamera(const float& width, const float& height)
    {
        return ast::PerspectiveCamera(width, height);
    }
} // namespace

struct SceneMain::Internal
{
    ast::PerspectiveCamera camera;
    std::vector<ast::StaticMeshInstance> staticMeshes;

    Internal(const float& screenWidth, const float& screenHeight)
        : camera(::createCamera(screenWidth, screenHeight)) {}

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

    void update(const float& delta)
    {
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
};

SceneMain::SceneMain(const float& screenWidth, const float& screenHeight)
    : internal(ast::make_internal_ptr<Internal>(screenWidth, screenHeight)) {}

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

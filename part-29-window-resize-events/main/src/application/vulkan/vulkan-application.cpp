#include "vulkan-application.hpp"
#include "../../core/graphics-wrapper.hpp"
#include "../../core/sdl-wrapper.hpp"
#include "../../scene/scene-main.hpp"
#include "vulkan-context.hpp"

using ast::VulkanApplication;

namespace
{
    std::unique_ptr<ast::Scene> createMainScene(ast::VulkanContext& context)
    {
        std::unique_ptr<ast::Scene> scene{std::make_unique<ast::SceneMain>(context.getCurrentWindowSize())};
        context.loadAssetManifest(scene->getAssetManifest());
        scene->prepare();

        return scene;
    }
} // namespace

struct VulkanApplication::Internal
{
    ast::VulkanContext context;
    std::unique_ptr<ast::Scene> scene;

    Internal() : context(ast::VulkanContext()) {}

    ast::Scene& getScene()
    {
        if (!scene)
        {
            scene = ::createMainScene(context);
        }

        return *scene;
    }

    void update(const float& delta)
    {
        getScene().update(delta);
    }

    void render()
    {
        if (context.renderBegin())
        {
            getScene().render(context);
            context.renderEnd();
        }
    }

    void onWindowResized()
    {
        getScene().onWindowResized(context.getCurrentWindowSize());
    }
};

VulkanApplication::VulkanApplication() : internal(ast::make_internal_ptr<Internal>()) {}

void VulkanApplication::update(const float& delta)
{
    internal->update(delta);
}

void VulkanApplication::render()
{
    internal->render();
}

void VulkanApplication::onWindowResized()
{
    internal->onWindowResized();
}

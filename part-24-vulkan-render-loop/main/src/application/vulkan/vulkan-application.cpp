#include "vulkan-application.hpp"
#include "../../core/graphics-wrapper.hpp"
#include "../../core/sdl-wrapper.hpp"
#include "../../scene/scene-main.hpp"
#include "vulkan-asset-manager.hpp"
#include "vulkan-context.hpp"

using ast::VulkanApplication;

namespace
{
    std::shared_ptr<ast::VulkanAssetManager> createAssetManager()
    {
        return std::make_shared<ast::VulkanAssetManager>(ast::VulkanAssetManager());
    }

    std::unique_ptr<ast::Scene> createMainScene(ast::AssetManager& assetManager)
    {
        std::pair<uint32_t, uint32_t> displaySize{ast::sdl::getDisplaySize()};
        std::unique_ptr<ast::Scene> scene{std::make_unique<ast::SceneMain>(
            static_cast<float>(displaySize.first),
            static_cast<float>(displaySize.second))};
        scene->prepare(assetManager);
        return scene;
    }
} // namespace

struct VulkanApplication::Internal
{
    const std::shared_ptr<ast::VulkanAssetManager> assetManager;
    ast::VulkanContext context;
    std::unique_ptr<ast::Scene> scene;

    Internal() : assetManager(::createAssetManager()),
                 context(ast::VulkanContext(assetManager)) {}

    ast::Scene& getScene()
    {
        if (!scene)
        {
            scene = ::createMainScene(*assetManager);
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

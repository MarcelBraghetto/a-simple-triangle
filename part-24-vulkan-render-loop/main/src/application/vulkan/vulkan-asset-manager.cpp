#include "vulkan-asset-manager.hpp"
#include "../../core/assets.hpp"

using ast::VulkanAssetManager;

struct VulkanAssetManager::Internal
{
    Internal() {}
};

VulkanAssetManager::VulkanAssetManager() : internal(ast::make_internal_ptr<Internal>()) {}

void VulkanAssetManager::loadPipelines(const std::vector<ast::assets::Pipeline>& pipelines)
{
    // TODO: Implement me
}

void VulkanAssetManager::loadStaticMeshes(const std::vector<ast::assets::StaticMesh>& staticMeshes)
{
    // TODO: Implement me
}

void VulkanAssetManager::loadTextures(const std::vector<ast::assets::Texture>& textures)
{
    // TODO: Implement me
}

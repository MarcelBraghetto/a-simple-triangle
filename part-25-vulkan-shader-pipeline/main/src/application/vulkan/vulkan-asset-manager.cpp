#include "vulkan-asset-manager.hpp"
#include "../../core/assets.hpp"
#include "../../core/log.hpp"
#include "vulkan-pipeline.hpp"
#include <unordered_map>

using ast::VulkanAssetManager;

namespace
{
    ast::VulkanPipeline createPipeline(const ast::assets::Pipeline& pipeline,
                                       const ast::VulkanPhysicalDevice& physicalDevice,
                                       const ast::VulkanDevice& device,
                                       const ast::VulkanRenderContext& renderContext)
    {
        const std::string pipelinePath{ast::assets::resolvePipelinePath(pipeline)};

        ast::log("ast::VulkanAssetManager::createPipeline", "Creating pipeline: " + pipelinePath);

        return ast::VulkanPipeline(physicalDevice,
                                   device,
                                   pipelinePath,
                                   renderContext.getViewport(),
                                   renderContext.getScissor(),
                                   renderContext.getRenderPass());
    }
} // namespace

struct VulkanAssetManager::Internal
{
    std::unordered_map<ast::assets::Pipeline, ast::VulkanPipeline> pipelineCache;

    Internal() {}

    void loadAssetManifest(const ast::VulkanPhysicalDevice& physicalDevice,
                           const ast::VulkanDevice& device,
                           const ast::VulkanRenderContext& renderContext,
                           const ast::AssetManifest& assetManifest)
    {
        for (const auto& pipeline : assetManifest.pipelines)
        {
            if (pipelineCache.count(pipeline) == 0)
            {
                pipelineCache.insert(std::make_pair(
                    pipeline,
                    ::createPipeline(pipeline, physicalDevice, device, renderContext)));
            }
        }
    }

    void reloadContextualAssets(const ast::VulkanPhysicalDevice& physicalDevice,
                                const ast::VulkanDevice& device,
                                const ast::VulkanRenderContext& renderContext)
    {
        for (auto& element : pipelineCache)
        {
            element.second = ::createPipeline(element.first, physicalDevice, device, renderContext);
        }
    }
};

VulkanAssetManager::VulkanAssetManager() : internal(ast::make_internal_ptr<Internal>()) {}

void VulkanAssetManager::loadAssetManifest(const ast::VulkanPhysicalDevice& physicalDevice,
                                           const ast::VulkanDevice& device,
                                           const ast::VulkanRenderContext& renderContext,
                                           const ast::AssetManifest& assetManifest)
{
    internal->loadAssetManifest(physicalDevice, device, renderContext, assetManifest);
}

void VulkanAssetManager::reloadContextualAssets(const ast::VulkanPhysicalDevice& physicalDevice,
                                                const ast::VulkanDevice& device,
                                                const ast::VulkanRenderContext& renderContext)
{
    internal->reloadContextualAssets(physicalDevice, device, renderContext);
}

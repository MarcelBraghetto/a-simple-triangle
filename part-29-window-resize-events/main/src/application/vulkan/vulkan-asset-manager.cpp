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

    ast::VulkanMesh createMesh(const ast::VulkanPhysicalDevice& physicalDevice,
                               const ast::VulkanDevice& device,
                               const ast::VulkanCommandPool& commandPool,
                               const ast::assets::StaticMesh& staticMesh)
    {
        std::string meshPath{ast::assets::resolveStaticMeshPath(staticMesh)};

        ast::log("ast::VulkanAssetManager::createMesh", "Creating static mesh from " + meshPath);

        return ast::VulkanMesh(physicalDevice,
                               device,
                               commandPool,
                               ast::assets::loadOBJFile(meshPath));
    }

    ast::VulkanTexture createTexture(const ast::assets::Texture& texture,
                                     const ast::VulkanPhysicalDevice& physicalDevice,
                                     const ast::VulkanDevice& device,
                                     const ast::VulkanCommandPool& commandPool,
                                     const ast::VulkanRenderContext& renderContext)
    {
        std::string texturePath{ast::assets::resolveTexturePath(texture)};

        ast::log("ast::VulkanAssetManager::createTexture", "Creating texture from " + texturePath);

        ast::Bitmap bitmap{ast::assets::loadBitmap(texturePath)};

        return ast::VulkanTexture(texture,
                                  physicalDevice,
                                  device,
                                  commandPool,
                                  bitmap);
    }
} // namespace

struct VulkanAssetManager::Internal
{
    std::unordered_map<ast::assets::Pipeline, ast::VulkanPipeline> pipelineCache;
    std::unordered_map<ast::assets::StaticMesh, ast::VulkanMesh> staticMeshCache;
    std::unordered_map<ast::assets::Texture, ast::VulkanTexture> textureCache;

    Internal() {}

    void loadAssetManifest(const ast::VulkanPhysicalDevice& physicalDevice,
                           const ast::VulkanDevice& device,
                           const ast::VulkanRenderContext& renderContext,
                           const ast::VulkanCommandPool& commandPool,
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

        for (const auto& staticMesh : assetManifest.staticMeshes)
        {
            if (staticMeshCache.count(staticMesh) == 0)
            {
                staticMeshCache.insert(std::make_pair(
                    staticMesh,
                    ::createMesh(physicalDevice, device, commandPool, staticMesh)));
            }
        }

        for (const auto& texture : assetManifest.textures)
        {
            if (textureCache.count(texture) == 0)
            {
                textureCache.insert(std::make_pair(
                    texture,
                    ::createTexture(texture, physicalDevice, device, commandPool, renderContext)));
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
                                           const ast::VulkanCommandPool& commandPool,
                                           const ast::AssetManifest& assetManifest)
{
    internal->loadAssetManifest(physicalDevice, device, renderContext, commandPool, assetManifest);
}

void VulkanAssetManager::reloadContextualAssets(const ast::VulkanPhysicalDevice& physicalDevice,
                                                const ast::VulkanDevice& device,
                                                const ast::VulkanRenderContext& renderContext)
{
    internal->reloadContextualAssets(physicalDevice, device, renderContext);
}

const ast::VulkanPipeline& VulkanAssetManager::getPipeline(const ast::assets::Pipeline& pipeline) const
{
    return internal->pipelineCache.at(pipeline);
}

const ast::VulkanMesh& VulkanAssetManager::getStaticMesh(const ast::assets::StaticMesh& staticMesh) const
{
    return internal->staticMeshCache.at(staticMesh);
}

const ast::VulkanTexture& VulkanAssetManager::getTexture(const ast::assets::Texture& texture) const
{
    return internal->textureCache.at(texture);
}

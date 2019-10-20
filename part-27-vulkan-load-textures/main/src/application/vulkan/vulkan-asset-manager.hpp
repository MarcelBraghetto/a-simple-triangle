#pragma once

#include "../../core/asset-manifest.hpp"
#include "../../core/internal-ptr.hpp"
#include "vulkan-command-pool.hpp"
#include "vulkan-device.hpp"
#include "vulkan-mesh.hpp"
#include "vulkan-physical-device.hpp"
#include "vulkan-render-context.hpp"
#include "vulkan-texture.hpp"

namespace ast
{
    struct VulkanAssetManager
    {
        VulkanAssetManager();

        void loadAssetManifest(const ast::VulkanPhysicalDevice& physicalDevice,
                               const ast::VulkanDevice& device,
                               const ast::VulkanRenderContext& renderContext,
                               const ast::VulkanCommandPool& commandPool,
                               const ast::AssetManifest& assetManifest);

        void reloadContextualAssets(const ast::VulkanPhysicalDevice& physicalDevice,
                                    const ast::VulkanDevice& device,
                                    const ast::VulkanRenderContext& renderContext);

        const ast::VulkanMesh& getStaticMesh(const ast::assets::StaticMesh& staticMesh) const;

        const ast::VulkanTexture& getTexture(const ast::assets::Texture& texture) const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

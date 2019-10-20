#pragma once

#include "../../core/asset-manifest.hpp"
#include "../../core/internal-ptr.hpp"
#include "vulkan-render-context.hpp"

namespace ast
{
    struct VulkanAssetManager
    {
        VulkanAssetManager();

        void loadAssetManifest(const ast::VulkanPhysicalDevice& physicalDevice,
                               const ast::VulkanDevice& device,
                               const ast::VulkanRenderContext& renderContext,
                               const ast::AssetManifest& assetManifest);

        void reloadContextualAssets(const ast::VulkanPhysicalDevice& physicalDevice,
                                    const ast::VulkanDevice& device,
                                    const ast::VulkanRenderContext& renderContext);

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

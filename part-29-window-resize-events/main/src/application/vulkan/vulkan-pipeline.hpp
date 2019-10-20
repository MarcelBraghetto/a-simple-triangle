#pragma once

#include "../../core/graphics-wrapper.hpp"
#include "../../core/internal-ptr.hpp"
#include "../../core/static-mesh-instance.hpp"
#include "vulkan-device.hpp"
#include "vulkan-physical-device.hpp"
#include <string>
#include <vector>

namespace ast
{
    struct VulkanAssetManager;

    struct VulkanPipeline
    {
        VulkanPipeline(const ast::VulkanPhysicalDevice& physicalDevice,
                       const ast::VulkanDevice& device,
                       const std::string& shaderName,
                       const vk::Viewport& viewport,
                       const vk::Rect2D& scissor,
                       const vk::RenderPass& renderPass);

        void render(const ast::VulkanDevice& device,
                    const vk::CommandBuffer& commandBuffer,
                    const ast::VulkanAssetManager& assetManager,
                    const std::vector<ast::StaticMeshInstance>& staticMeshInstances) const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

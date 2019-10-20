#pragma once

#include "../../core/internal-ptr.hpp"
#include "vulkan-device.hpp"
#include "vulkan-physical-device.hpp"
#include "vulkan-swapchain.hpp"

namespace ast
{
    struct VulkanRenderPass
    {
        VulkanRenderPass(const ast::VulkanPhysicalDevice& physicalDevice,
                         const ast::VulkanDevice& device,
                         const ast::VulkanSwapchain& swapchain);

		const vk::RenderPass& getRenderPass() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

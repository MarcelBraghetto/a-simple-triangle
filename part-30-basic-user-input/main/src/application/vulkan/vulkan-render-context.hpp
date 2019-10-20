#pragma once

#include "../../core/internal-ptr.hpp"
#include "../../core/sdl-window.hpp"
#include "vulkan-command-pool.hpp"
#include "vulkan-device.hpp"
#include "vulkan-physical-device.hpp"
#include "vulkan-surface.hpp"

namespace ast
{
    struct VulkanRenderContext
    {
        VulkanRenderContext(const ast::SDLWindow& window,
                            const ast::VulkanPhysicalDevice& physicalDevice,
                            const ast::VulkanDevice& device,
                            const ast::VulkanSurface& surface,
                            const ast::VulkanCommandPool& commandPool,
                            const vk::SwapchainKHR& oldSwapchain = vk::SwapchainKHR());

        bool renderBegin(const ast::VulkanDevice& device);

        bool renderEnd(const ast::VulkanDevice& device);

        ast::VulkanRenderContext recreate(const ast::SDLWindow& window,
                                          const ast::VulkanPhysicalDevice& physicalDevice,
                                          const ast::VulkanDevice& device,
                                          const ast::VulkanSurface& surface,
                                          const ast::VulkanCommandPool& commandPool);

        const vk::Viewport& getViewport() const;

        const vk::Rect2D& getScissor() const;

        const vk::RenderPass& getRenderPass() const;

        const vk::CommandBuffer& getActiveCommandBuffer() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

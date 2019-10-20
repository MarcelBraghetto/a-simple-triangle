#include "vulkan-render-context.hpp"
#include "vulkan-render-pass.hpp"
#include "vulkan-swapchain.hpp"

using ast::VulkanRenderContext;

struct VulkanRenderContext::Internal
{
    const ast::VulkanSwapchain swapchain;
    const ast::VulkanRenderPass renderPass;

    Internal(const ast::SDLWindow& window,
             const ast::VulkanPhysicalDevice& physicalDevice,
             const ast::VulkanDevice& device,
             const ast::VulkanSurface& surface)
        : swapchain(ast::VulkanSwapchain(window, physicalDevice, device, surface)),
          renderPass(ast::VulkanRenderPass(physicalDevice, device, swapchain)) {}
};

VulkanRenderContext::VulkanRenderContext(const ast::SDLWindow& window,
                                         const ast::VulkanPhysicalDevice& physicalDevice,
                                         const ast::VulkanDevice& device,
                                         const ast::VulkanSurface& surface)
    : internal(ast::make_internal_ptr<Internal>(window, physicalDevice, device, surface)) {}

#pragma once

#include "../../core/graphics-wrapper.hpp"
#include "../../core/internal-ptr.hpp"
#include "../../core/sdl-window.hpp"
#include "vulkan-device.hpp"
#include "vulkan-image-view.hpp"
#include "vulkan-physical-device.hpp"
#include "vulkan-surface.hpp"
#include <vector>

namespace ast
{
    struct VulkanSwapchain
    {
        VulkanSwapchain(const ast::SDLWindow& window,
                        const ast::VulkanPhysicalDevice& physicalDevice,
                        const ast::VulkanDevice& device,
                        const ast::VulkanSurface& surface);

        const vk::SwapchainKHR& getSwapchain() const;

        const std::vector<ast::VulkanImageView>& getImageViews() const;

		const vk::Format& getColorFormat() const;

		const vk::Extent2D& getExtent() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

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
                            const ast::VulkanCommandPool& commandPool);

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

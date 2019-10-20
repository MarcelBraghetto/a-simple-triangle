#pragma once

#include "../../core/graphics-wrapper.hpp"
#include "../../core/internal-ptr.hpp"
#include "../../core/sdl-window.hpp"
#include "vulkan-physical-device.hpp"

namespace ast
{
    struct VulkanSurface
    {
        VulkanSurface(const vk::Instance& instance,
                      const ast::VulkanPhysicalDevice& physicalDevice,
                      const ast::SDLWindow& window);

        const vk::SurfaceKHR& getSurface() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

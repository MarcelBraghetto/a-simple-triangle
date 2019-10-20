#pragma once

#include "../../core/graphics-wrapper.hpp"
#include "../../core/internal-ptr.hpp"
#include "vulkan-physical-device.hpp"
#include "vulkan-surface.hpp"

namespace ast
{
    struct VulkanDevice
    {
        VulkanDevice(const ast::VulkanPhysicalDevice& physicalDevice,
                     const ast::VulkanSurface& surface);

        const vk::Device& getDevice() const;

        uint32_t getGraphicsQueueIndex() const;

        uint32_t getPresentationQueueIndex() const;

        bool hasDiscretePresentationQueue() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

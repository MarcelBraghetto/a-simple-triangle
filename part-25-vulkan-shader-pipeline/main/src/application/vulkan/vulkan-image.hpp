#pragma once

#include "../../core/graphics-wrapper.hpp"
#include "../../core/internal-ptr.hpp"
#include "vulkan-command-pool.hpp"
#include "vulkan-device.hpp"
#include "vulkan-physical-device.hpp"

namespace ast
{
    struct VulkanImage
    {
        VulkanImage(const ast::VulkanCommandPool& commandPool,
                    const ast::VulkanPhysicalDevice& physicalDevice,
                    const ast::VulkanDevice& device,
                    const uint32_t& width,
                    const uint32_t& height,
                    const uint32_t& mipLevels,
                    const vk::SampleCountFlagBits& sampleCount,
                    const vk::Format& format,
                    const vk::ImageTiling& tiling,
                    const vk::ImageUsageFlags& usageFlags,
                    const vk::MemoryPropertyFlags& memoryFlags,
                    const vk::ImageLayout& oldLayout,
                    const vk::ImageLayout& newLayout);

        uint32_t getWidth() const;

        uint32_t getHeight() const;

        uint32_t getMipLevels() const;

        const vk::Format& getFormat() const;

        const vk::Image& getImage() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

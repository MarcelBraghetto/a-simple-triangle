#pragma once

#include "../../core/graphics-wrapper.hpp"
#include "../../core/internal-ptr.hpp"
#include "vulkan-command-pool.hpp"
#include "vulkan-device.hpp"
#include "vulkan-physical-device.hpp"

namespace ast
{
    struct VulkanBuffer
    {
        VulkanBuffer(const ast::VulkanPhysicalDevice& physicalDevice,
                     const ast::VulkanDevice& device,
                     const vk::DeviceSize& size,
                     const vk::BufferUsageFlags& bufferFlags,
                     const vk::MemoryPropertyFlags& memoryFlags,
                     const void* dataSource);

        const vk::Buffer& getBuffer() const;

        static ast::VulkanBuffer createDeviceLocalBuffer(const ast::VulkanPhysicalDevice& physicalDevice,
                                                         const ast::VulkanDevice& device,
                                                         const ast::VulkanCommandPool& commandPool,
                                                         const vk::DeviceSize& size,
                                                         const vk::BufferUsageFlags& bufferFlags,
                                                         const void* dataSource);

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

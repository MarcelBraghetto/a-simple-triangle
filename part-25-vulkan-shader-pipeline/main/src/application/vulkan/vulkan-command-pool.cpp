#include "vulkan-command-pool.hpp"

using ast::VulkanCommandPool;

namespace
{
    vk::UniqueCommandPool createCommandPool(const ast::VulkanDevice& device)
    {
        vk::CommandPoolCreateInfo info{
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            device.getGraphicsQueueIndex()};

        return device.getDevice().createCommandPoolUnique(info);
    }

    vk::UniqueCommandBuffer beginCommandBuffer(const vk::CommandPool& commandPool,
                                               const ast::VulkanDevice& device)
    {
        // Describe how to create our command buffer - we only create 1.
        vk::CommandBufferAllocateInfo allocateInfo{
            commandPool,                      // Command pool
            vk::CommandBufferLevel::ePrimary, // Level
            1};                               // Command buffer count

        // Create and move the first (and only) command buffer that gets created by the device.
        vk::UniqueCommandBuffer commandBuffer{
            std::move(device.getDevice().allocateCommandBuffersUnique(allocateInfo)[0])};

        // Define how this command buffer should begin.
        vk::CommandBufferBeginInfo beginInfo{
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit, // Flags
            nullptr                                         // Inheritance info
        };

        // Request the command buffer to begin itself.
        commandBuffer->begin(beginInfo);

        // The caller will now take ownership of the command buffer and is
        // responsible for invoking the 'endCommandBuffer' upon it.
        return commandBuffer;
    }

    void endCommandBuffer(const vk::CommandBuffer& commandBuffer,
                          const ast::VulkanDevice& device)
    {
        // Ask the command buffer to end itself.
        commandBuffer.end();

        // Configure a submission object to send to the graphics queue to wait
        // for the command buffer to have been completed.
        vk::SubmitInfo submitInfo{
            0,              // Wait semaphore count
            nullptr,        // Wait semaphores
            nullptr,        // Wait destination stage mask
            1,              // Command buffer count
            &commandBuffer, // Command buffers,
            0,              // Signal semaphore count
            nullptr         // Signal semaphores
        };

        // Ask the graphics queue to take the submission object which will declare
        // the command buffer to wait on, then wait until the graphics queue is
        // idle, indicating that the command buffer is complete.
        device.getGraphicsQueue().submit(1, &submitInfo, vk::Fence());
        device.getGraphicsQueue().waitIdle();
    }

    std::vector<vk::UniqueCommandBuffer> createCommandBuffers(const vk::Device& device,
                                                              const vk::CommandPool& commandPool,
                                                              const uint32_t& count)
    {
        vk::CommandBufferAllocateInfo info{
            commandPool,                      // Command pool
            vk::CommandBufferLevel::ePrimary, // Level
            count                             // Command buffer count
        };

        return device.allocateCommandBuffersUnique(info);
    }
} // namespace

struct VulkanCommandPool::Internal
{
    const vk::UniqueCommandPool commandPool;

    Internal(const ast::VulkanDevice& device)
        : commandPool(::createCommandPool(device)) {}
};

VulkanCommandPool::VulkanCommandPool(const ast::VulkanDevice& device)
    : internal(ast::make_internal_ptr<Internal>(device)) {}

vk::UniqueCommandBuffer VulkanCommandPool::beginCommandBuffer(const ast::VulkanDevice& device) const
{
    return ::beginCommandBuffer(internal->commandPool.get(), device);
}

void VulkanCommandPool::endCommandBuffer(const vk::CommandBuffer& commandBuffer,
                                         const ast::VulkanDevice& device) const
{
    ::endCommandBuffer(commandBuffer, device);
}

std::vector<vk::UniqueCommandBuffer> VulkanCommandPool::createCommandBuffers(const ast::VulkanDevice& device,
                                                                             const uint32_t& count) const
{
    return ::createCommandBuffers(device.getDevice(), internal->commandPool.get(), count);
}

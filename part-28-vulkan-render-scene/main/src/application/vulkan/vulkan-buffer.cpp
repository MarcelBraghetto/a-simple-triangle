#include "vulkan-buffer.hpp"

using ast::VulkanBuffer;

namespace
{
    vk::UniqueBuffer createBuffer(const ast::VulkanDevice& device,
                                  const vk::DeviceSize& size,
                                  const vk::BufferUsageFlags& bufferFlags)
    {
        vk::BufferCreateInfo info{
            vk::BufferCreateFlags(),     // Flags
            size,                        // Size
            bufferFlags,                 // Buffer usage flags
            vk::SharingMode::eExclusive, // Sharing mode
            0,                           // Queue family index count
            nullptr};                    // Queue family indices

        return device.getDevice().createBufferUnique(info);
    }

    vk::UniqueDeviceMemory allocateMemory(const ast::VulkanPhysicalDevice& physicalDevice,
                                          const ast::VulkanDevice& device,
                                          const vk::Buffer& buffer,
                                          const vk::MemoryPropertyFlags& memoryFlags)
    {
        vk::MemoryRequirements memoryRequirements{
            device.getDevice().getBufferMemoryRequirements(buffer)};

        uint32_t memoryTypeIndex{
            physicalDevice.getMemoryTypeIndex(memoryRequirements.memoryTypeBits, memoryFlags)};

        vk::MemoryAllocateInfo info{
            memoryRequirements.size, // Allocation size
            memoryTypeIndex};        // Memory type index

        return device.getDevice().allocateMemoryUnique(info);
    }
} // namespace

struct VulkanBuffer::Internal
{
    const vk::UniqueBuffer buffer;
    const vk::UniqueDeviceMemory deviceMemory;

    Internal(const ast::VulkanPhysicalDevice& physicalDevice,
             const ast::VulkanDevice& device,
             const vk::DeviceSize& size,
             const vk::BufferUsageFlags& bufferFlags,
             const vk::MemoryPropertyFlags& memoryFlags,
             const void* dataSource)
        : buffer(::createBuffer(device, size, bufferFlags)),
          deviceMemory(::allocateMemory(physicalDevice, device, buffer.get(), memoryFlags))
    {
        // Take the buffer and the allocated memory and bind them together.
        device.getDevice().bindBufferMemory(buffer.get(), deviceMemory.get(), 0);

        // Take the datasource and copy it into our allocated memory block.
        if (dataSource)
        {
            void* mappedMemory{device.getDevice().mapMemory(deviceMemory.get(), 0, size)};
            std::memcpy(mappedMemory, dataSource, static_cast<size_t>(size));
            device.getDevice().unmapMemory(deviceMemory.get());
        }
    }
};

VulkanBuffer::VulkanBuffer(const ast::VulkanPhysicalDevice& physicalDevice,
                           const ast::VulkanDevice& device,
                           const vk::DeviceSize& size,
                           const vk::BufferUsageFlags& bufferFlags,
                           const vk::MemoryPropertyFlags& memoryFlags,
                           const void* dataSource)
    : internal(ast::make_internal_ptr<Internal>(physicalDevice, device, size, bufferFlags, memoryFlags, dataSource)) {}

const vk::Buffer& VulkanBuffer::getBuffer() const
{
    return internal->buffer.get();
}

ast::VulkanBuffer VulkanBuffer::createDeviceLocalBuffer(const ast::VulkanPhysicalDevice& physicalDevice,
                                                        const ast::VulkanDevice& device,
                                                        const ast::VulkanCommandPool& commandPool,
                                                        const vk::DeviceSize& size,
                                                        const vk::BufferUsageFlags& bufferFlags,
                                                        const void* dataSource)
{
    // Begin by creating a staging buffer with memory flags that allow us to perform
    // memory operations, and that is marked as being a 'transfer source' so we can
    // transition it to a device local buffer afterward.
    ast::VulkanBuffer stagingBuffer{
        physicalDevice,
        device,
        size,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
        dataSource};

    // Create the device local buffer with no data source but which is marked as being a
    // 'transfer destination' and adopts whatever additional buffer flags the caller
    // specified. The memory properties we want for this one is to be in device local memory.
    ast::VulkanBuffer deviceLocalBuffer{
        physicalDevice,
        device,
        size,
        vk::BufferUsageFlagBits::eTransferDst | bufferFlags,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        nullptr};

    // Initiate a new command buffer to perform the buffer copy operation between the
    // staging buffer and the device local buffer.
    vk::UniqueCommandBuffer commandBuffer{commandPool.beginCommandBuffer(device)};

    // Define what region of the two buffers should participate in the copy operation.
    vk::BufferCopy copyRegion{
        0,     // Source offset
        0,     // Destination offset
        size}; // Size

    // Request the command buffer to perform the copy buffer operation, giving it the
    // staging and device local buffers along with the region in which to copy across.
    commandBuffer->copyBuffer(stagingBuffer.getBuffer(), deviceLocalBuffer.getBuffer(), 1, &copyRegion);

    // End the command buffer, causing it to be carried out.
    commandPool.endCommandBuffer(commandBuffer.get(), device);

    // Hand back the buffer that is now populated into device local memory.
    return deviceLocalBuffer;
}

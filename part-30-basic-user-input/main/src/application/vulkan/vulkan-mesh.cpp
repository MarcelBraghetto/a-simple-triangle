#include "vulkan-mesh.hpp"
#include "vulkan-buffer.hpp"
#include <vector>

using ast::VulkanMesh;

namespace
{
    ast::VulkanBuffer createVertexBuffer(const ast::VulkanPhysicalDevice& physicalDevice,
                                         const ast::VulkanDevice& device,
                                         const ast::VulkanCommandPool& commandPool,
                                         const ast::Mesh& mesh)
    {
        return ast::VulkanBuffer::createDeviceLocalBuffer(physicalDevice,
                                                          device,
                                                          commandPool,
                                                          sizeof(ast::Vertex) * mesh.getNumVertices(),
                                                          vk::BufferUsageFlagBits::eVertexBuffer,
                                                          mesh.getVertices().data());
    }

    ast::VulkanBuffer createIndexBuffer(const ast::VulkanPhysicalDevice& physicalDevice,
                                        const ast::VulkanDevice& device,
                                        const ast::VulkanCommandPool& commandPool,
                                        const ast::Mesh& mesh)
    {
        return ast::VulkanBuffer::createDeviceLocalBuffer(physicalDevice,
                                                          device,
                                                          commandPool,
                                                          sizeof(uint32_t) * mesh.getNumIndices(),
                                                          vk::BufferUsageFlagBits::eIndexBuffer,
                                                          mesh.getIndices().data());
    }
} // namespace

struct VulkanMesh::Internal
{
    const ast::VulkanBuffer vertexBuffer;
    const ast::VulkanBuffer indexBuffer;
    const uint32_t numIndices;

    Internal(const ast::VulkanPhysicalDevice& physicalDevice,
             const ast::VulkanDevice& device,
             const ast::VulkanCommandPool& commandPool,
             const ast::Mesh& mesh)
        : vertexBuffer(::createVertexBuffer(physicalDevice, device, commandPool, mesh)),
          indexBuffer(::createIndexBuffer(physicalDevice, device, commandPool, mesh)),
          numIndices(mesh.getNumIndices()) {}
};

VulkanMesh::VulkanMesh(const ast::VulkanPhysicalDevice& physicalDevice,
                       const ast::VulkanDevice& device,
                       const ast::VulkanCommandPool& commandPool,
                       const ast::Mesh& mesh)
    : internal(ast::make_internal_ptr<Internal>(physicalDevice, device, commandPool, mesh)) {}

const vk::Buffer& VulkanMesh::getVertexBuffer() const
{
    return internal->vertexBuffer.getBuffer();
}

const vk::Buffer& VulkanMesh::getIndexBuffer() const
{
    return internal->indexBuffer.getBuffer();
}

const uint32_t& VulkanMesh::getNumIndices() const
{
    return internal->numIndices;
}

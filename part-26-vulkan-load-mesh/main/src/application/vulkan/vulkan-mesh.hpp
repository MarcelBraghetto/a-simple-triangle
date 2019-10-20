#pragma once

#include "../../core/graphics-wrapper.hpp"
#include "../../core/internal-ptr.hpp"
#include "../../core/mesh.hpp"
#include "vulkan-command-pool.hpp"
#include "vulkan-device.hpp"
#include "vulkan-physical-device.hpp"

namespace ast
{
    struct VulkanMesh
    {
        VulkanMesh(const ast::VulkanPhysicalDevice& physicalDevice,
                   const ast::VulkanDevice& device,
                   const ast::VulkanCommandPool& commandPool,
                   const ast::Mesh& mesh);

        const vk::Buffer& getVertexBuffer() const;

        const vk::Buffer& getIndexBuffer() const;

        const uint32_t& getNumIndices() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

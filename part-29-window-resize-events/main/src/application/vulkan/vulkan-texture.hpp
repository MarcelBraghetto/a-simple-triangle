#pragma once

#include "../../core/asset-inventory.hpp"
#include "../../core/bitmap.hpp"
#include "../../core/graphics-wrapper.hpp"
#include "../../core/internal-ptr.hpp"
#include "vulkan-command-pool.hpp"
#include "vulkan-device.hpp"
#include "vulkan-image-view.hpp"
#include "vulkan-physical-device.hpp"

namespace ast
{
    struct VulkanTexture
    {
        VulkanTexture(const ast::assets::Texture& textureId,
                      const ast::VulkanPhysicalDevice& physicalDevice,
                      const ast::VulkanDevice& device,
                      const ast::VulkanCommandPool& commandPool,
                      const ast::Bitmap& bitmap);

        const ast::assets::Texture& getTextureId() const;

        const ast::VulkanImageView& getImageView() const;

        const vk::Sampler& getSampler() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

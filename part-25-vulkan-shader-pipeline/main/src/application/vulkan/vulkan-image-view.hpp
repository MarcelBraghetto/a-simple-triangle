#pragma once

#include "../../core/graphics-wrapper.hpp"
#include "../../core/internal-ptr.hpp"

namespace ast
{
    struct VulkanImageView
    {
        VulkanImageView(const vk::Device& device,
                        const vk::Image& image,
                        const vk::Format& format,
                        const vk::ImageAspectFlags& aspectFlags,
                        const uint32_t& mipLevels);

        const vk::ImageView& getImageView() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

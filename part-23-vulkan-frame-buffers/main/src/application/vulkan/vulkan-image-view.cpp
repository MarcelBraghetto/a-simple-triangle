#include "vulkan-image-view.hpp"

using ast::VulkanImageView;

namespace
{
    vk::UniqueImageView createImageView(const vk::Device& device,
                                        const vk::Image& image,
                                        const vk::Format& format,
                                        const vk::ImageAspectFlags& aspectFlags,
                                        const uint32_t& mipLevels)
    {
        vk::ImageSubresourceRange subresourceRangeInfo{
            aspectFlags, // Flags
            0,           // Base mip level
            mipLevels,   // Mip level count
            0,           // Base array layer
            1            // Layer count
        };

        vk::ImageViewCreateInfo createInfo{
            vk::ImageViewCreateFlags(), // Flags
            image,                      // Image
            vk::ImageViewType::e2D,     // View type
            format,                     // Format
            vk::ComponentMapping(),     // Components
            subresourceRangeInfo        // Subresource range
        };

        return device.createImageViewUnique(createInfo);
    }
} // namespace

struct VulkanImageView::Internal
{
    const vk::UniqueImageView imageView;

    Internal(const vk::Device& device,
             const vk::Image& image,
             const vk::Format& format,
             const vk::ImageAspectFlags& aspectFlags,
             const uint32_t& mipLevels)
        : imageView(::createImageView(device, image, format, aspectFlags, mipLevels)) {}
};

VulkanImageView::VulkanImageView(const vk::Device& device,
                                 const vk::Image& image,
                                 const vk::Format& format,
                                 const vk::ImageAspectFlags& aspectFlags,
                                 const uint32_t& mipLevels)
    : internal(ast::make_internal_ptr<Internal>(device, image, format, aspectFlags, mipLevels)) {}

const vk::ImageView& VulkanImageView::getImageView() const
{
    return internal->imageView.get();
}
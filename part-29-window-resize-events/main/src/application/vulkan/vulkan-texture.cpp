#include "vulkan-texture.hpp"
#include "vulkan-buffer.hpp"
#include "vulkan-image.hpp"
#include <cmath>

using ast::VulkanTexture;

namespace
{
    void generateMipMaps(const ast::VulkanDevice& device,
                         const ast::VulkanCommandPool& commandPool,
                         const ast::VulkanImage& image)
    {
        vk::ImageSubresourceRange barrierSubresourceRange{
            vk::ImageAspectFlagBits::eColor, // Aspect mask
            0,                               // Base mip level
            1,                               // Level count
            0,                               // Base array layer
            1};                              // Layer count

        // Setup a barrier object that we will reuse throughout the mip mapping code.
        vk::ImageMemoryBarrier barrier{
            vk::AccessFlags(),           // Source access mask
            vk::AccessFlags(),           // Destination access mask
            vk::ImageLayout::eUndefined, // Old layout
            vk::ImageLayout::eUndefined, // New layout
            VK_QUEUE_FAMILY_IGNORED,     // Source queue family index
            VK_QUEUE_FAMILY_IGNORED,     // Destination queue family index
            image.getImage(),            // Image
            barrierSubresourceRange};    // Subresource range

        // Start a new command buffer to generate the mipmaps.
        vk::UniqueCommandBuffer commandBuffer{commandPool.beginCommandBuffer(device)};

        // Our first mip size will be the same size as the image.
        int32_t mipWidth{static_cast<int32_t>(image.getWidth())};
        int32_t mipHeight{static_cast<int32_t>(image.getHeight())};
        uint32_t mipLevels{image.getMipLevels()};

        for (uint32_t mipLevel = 1; mipLevel < mipLevels; mipLevel++)
        {
            barrier.subresourceRange.baseMipLevel = mipLevel - 1;
            barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
            barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

            commandBuffer->pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                                           vk::PipelineStageFlagBits::eTransfer,
                                           vk::DependencyFlags(),
                                           0, nullptr,
                                           0, nullptr,
                                           1, &barrier);

            vk::ImageSubresourceLayers sourceSubresource{
                vk::ImageAspectFlagBits::eColor, // Aspect mask
                mipLevel - 1,                    // Mip level
                0,                               // Base array layer
                1};                              // Layer count

            std::array<vk::Offset3D, 2> sourceOffsets{
                vk::Offset3D{0, 0, 0},
                vk::Offset3D{mipWidth, mipHeight, 1}};

            vk::ImageSubresourceLayers destinationSubresource{
                vk::ImageAspectFlagBits::eColor, // Aspect mask
                mipLevel,                        // Mip level
                0,                               // Base array layer
                1};                              // Layer count

            std::array<vk::Offset3D, 2> destinationOffsets{
                vk::Offset3D{0, 0, 0},
                vk::Offset3D{mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1}};

            vk::ImageBlit blit{
                sourceSubresource,      // Source subresource
                sourceOffsets,          // Source offsets
                destinationSubresource, // Destination subresource
                destinationOffsets};    // Destination offsets

            commandBuffer->blitImage(image.getImage(), vk::ImageLayout::eTransferSrcOptimal,
                                     image.getImage(), vk::ImageLayout::eTransferDstOptimal,
                                     1, &blit,
                                     vk::Filter::eLinear);

            barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
            barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

            commandBuffer->pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                                           vk::PipelineStageFlagBits::eFragmentShader,
                                           vk::DependencyFlags(),
                                           0, nullptr,
                                           0, nullptr,
                                           1, &barrier);

            if (mipWidth > 1)
            {
                mipWidth /= 2;
            }

            if (mipHeight > 1)
            {
                mipHeight /= 2;
            }
        }

        barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
        barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

        commandBuffer->pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eFragmentShader,
            vk::DependencyFlags(),
            0, nullptr,
            0, nullptr,
            1, &barrier);

        commandPool.endCommandBuffer(commandBuffer.get(), device);
    }

    ast::VulkanImage createImage(const ast::VulkanPhysicalDevice& physicalDevice,
                                 const ast::VulkanDevice& device,
                                 const ast::VulkanCommandPool& commandPool,
                                 const ast::Bitmap& bitmap)
    {
        uint32_t imageWidth{bitmap.getWidth()};
        uint32_t imageHeight{bitmap.getHeight()};

        // We determine the mipmapping level by choosing the larger of the
        // width or height then finding the log base 2 of it which tells us
        // how many times we can multiply by 2 to reach the size.
        // For example if the image size is ( 1024 x 512 ):
        // 1. max ( 1024, 512 ) = 1024
        // 2. log2 ( 1024 ) = 10 (because 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 = 1024)
        // 3. floor ( 10 ) = 10
        // 4. add 1 to accommodate original full sized image = 11
        // mip levels = 11
        uint32_t mipLevels{static_cast<uint32_t>(std::floor(std::log2(std::max(imageWidth, imageHeight)))) + 1};

        // We expect that the supplied bitmap object is already in the
        // RGBA format therefore we know there will be 4 bytes per pixel.
        vk::DeviceSize bufferSize{imageWidth * imageHeight * 4};

        // Vulkan requires us to stage texture image data in a buffer then copy it into an
        // image object using Vulkan commands. We start by creating a buffer to hold the
        // raw bitmap data marked as a transfer source.
        ast::VulkanBuffer stagingBuffer{
            physicalDevice,
            device,
            bufferSize,
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            bitmap.getPixelData()};

        // We will create a new image object which we will mark as being able to be used as
        // both a transfer source and destination and that can be used to sample from which
        // is what our fragment shader will want to do with it via a sampler. We can't write
        // the actual pixel data directly into the image as Vulkan does not allow that. Instead
        // we must create the shell image and run a Vulkan command to copy the contents of the
        // staging buffer into the image memory area.
        ast::VulkanImage image{
            commandPool,
            physicalDevice,
            device,
            imageWidth,
            imageHeight,
            mipLevels,
            vk::SampleCountFlagBits::e1,
            vk::Format::eR8G8B8A8Unorm,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eSampled,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eTransferDstOptimal};

        // Kick off a new command buffer to copy the staging buffer to the image.
        vk::UniqueCommandBuffer commandBuffer{commandPool.beginCommandBuffer(device)};

        vk::ImageSubresourceLayers imageSubresource{
            vk::ImageAspectFlagBits::eColor, // Aspect mask
            0,                               // Mip level
            0,                               // Base array layer
            1};                              // Layer count

        vk::Extent3D imageExtent{
            imageWidth,  // Width
            imageHeight, // Height
            1};          // Depth

        vk::BufferImageCopy bufferImageCopy{
            0,                // Buffer offset
            0,                // Buffer row length
            0,                // Buffer image height
            imageSubresource, // Image subresource
            vk::Offset3D(),   // Image offset
            imageExtent};     // Image extent

        // Perform the copy operation from the staging buffer into the image.
        commandBuffer->copyBufferToImage(stagingBuffer.getBuffer(),
                                         image.getImage(),
                                         vk::ImageLayout::eTransferDstOptimal,
                                         1,
                                         &bufferImageCopy);

        commandPool.endCommandBuffer(commandBuffer.get(), device);

        ::generateMipMaps(device, commandPool, image);

        return image;
    }

    ast::VulkanImageView createImageView(const ast::VulkanDevice& device, const ast::VulkanImage& image)
    {
        return ast::VulkanImageView(device.getDevice(),
                                    image.getImage(),
                                    image.getFormat(),
                                    vk::ImageAspectFlagBits::eColor,
                                    image.getMipLevels());
    }

    vk::UniqueSampler createSampler(const ast::VulkanPhysicalDevice& physicalDevice,
                                    const ast::VulkanDevice& device,
                                    const ast::VulkanImage& image)
    {
        // Cast the mip levels of the image to a float for the max level of detail setting.
        float maxLod{static_cast<float>(image.getMipLevels())};

        // Determine whether we should apply anisotropic filtering.
        vk::Bool32 anisotropyEnabled = physicalDevice.isAnisotropicFilteringSupported() ? VK_TRUE : VK_FALSE;

        vk::SamplerCreateInfo info{
            vk::SamplerCreateFlags(),         // Flags
            vk::Filter::eLinear,              // Mag filter
            vk::Filter::eLinear,              // Min filter
            vk::SamplerMipmapMode::eLinear,   // Mipmap mode
            vk::SamplerAddressMode::eRepeat,  // Address mode U
            vk::SamplerAddressMode::eRepeat,  // Address mode V
            vk::SamplerAddressMode::eRepeat,  // Address mode W
            0.0f,                             // Mip LOD bias
            anisotropyEnabled,                // Anisotropy enabled
            anisotropyEnabled ? 8.0f : 1.0f,  // Max anisotropy
            VK_FALSE,                         // Compare enable
            vk::CompareOp::eNever,            // Compare op
            0.0f,                             // Min LOD
            maxLod,                           // Max LOD
            vk::BorderColor::eIntOpaqueBlack, // Border color
            VK_FALSE};                        // UnnormalizedCoordinates

        return device.getDevice().createSamplerUnique(info);
    }
} // namespace

struct VulkanTexture::Internal
{
    const ast::assets::Texture textureId;
    const ast::VulkanImage image;
    const ast::VulkanImageView imageView;
    const vk::UniqueSampler sampler;

    Internal(const ast::assets::Texture textureId,
             const ast::VulkanPhysicalDevice& physicalDevice,
             const ast::VulkanDevice& device,
             const ast::VulkanCommandPool& commandPool,
             const ast::Bitmap& bitmap)
        : textureId(textureId),
          image(::createImage(physicalDevice, device, commandPool, bitmap)),
          imageView(::createImageView(device, image)),
          sampler(::createSampler(physicalDevice, device, image)) {}
};

VulkanTexture::VulkanTexture(const ast::assets::Texture& textureId,
                             const ast::VulkanPhysicalDevice& physicalDevice,
                             const ast::VulkanDevice& device,
                             const ast::VulkanCommandPool& commandPool,
                             const ast::Bitmap& bitmap)
    : internal(ast::make_internal_ptr<Internal>(textureId,
                                                physicalDevice,
                                                device,
                                                commandPool,
                                                bitmap)) {}

const ast::assets::Texture& VulkanTexture::getTextureId() const
{
    return internal->textureId;
}

const ast::VulkanImageView& VulkanTexture::getImageView() const
{
    return internal->imageView;
}

const vk::Sampler& VulkanTexture::getSampler() const
{
    return internal->sampler.get();
}

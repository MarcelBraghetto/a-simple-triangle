#include "vulkan-render-context.hpp"
#include "vulkan-image-view.hpp"
#include "vulkan-image.hpp"
#include "vulkan-render-pass.hpp"
#include "vulkan-swapchain.hpp"
#include <vector>

using ast::VulkanRenderContext;

namespace
{
    ast::VulkanImage createMultiSampleImage(const ast::VulkanCommandPool& commandPool,
                                            const ast::VulkanPhysicalDevice& physicalDevice,
                                            const ast::VulkanDevice& device,
                                            const ast::VulkanSwapchain& swapchain)
    {
        const vk::Extent2D& extent{swapchain.getExtent()};

        return ast::VulkanImage(
            commandPool,
            physicalDevice,
            device,
            extent.width,
            extent.height,
            1,
            physicalDevice.getMultiSamplingLevel(),
            swapchain.getColorFormat(),
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eColorAttachmentOptimal);
    }

    ast::VulkanImageView createImageView(const ast::VulkanDevice& device,
                                         const ast::VulkanImage& image,
                                         const vk::ImageAspectFlags& aspectFlags)
    {
        return ast::VulkanImageView(device.getDevice(),
                                    image.getImage(),
                                    image.getFormat(),
                                    aspectFlags,
                                    image.getMipLevels());
    }

    ast::VulkanImage createDepthImage(const ast::VulkanCommandPool& commandPool,
                                      const ast::VulkanPhysicalDevice& physicalDevice,
                                      const ast::VulkanDevice& device,
                                      const ast::VulkanSwapchain& swapchain)
    {
        const vk::Extent2D& extent{swapchain.getExtent()};

        return ast::VulkanImage(
            commandPool,
            physicalDevice,
            device,
            extent.width,
            extent.height,
            1,
            physicalDevice.getMultiSamplingLevel(),
            physicalDevice.getDepthFormat(),
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eDepthStencilAttachment,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eDepthStencilAttachmentOptimal);
    }

    std::vector<vk::UniqueFramebuffer> createFramebuffers(const ast::VulkanDevice& device,
                                                          const ast::VulkanSwapchain& swapchain,
                                                          const ast::VulkanRenderPass& renderPass,
                                                          const ast::VulkanImageView& multiSampleImageView,
                                                          const ast::VulkanImageView& depthImageView)
    {
        std::vector<vk::UniqueFramebuffer> framebuffers;

        const vk::Extent2D& extent{swapchain.getExtent()};

        for (const auto& swapchainImageView : swapchain.getImageViews())
        {
            std::array<vk::ImageView, 3> attachments{
                multiSampleImageView.getImageView(),
                depthImageView.getImageView(),
                swapchainImageView.getImageView()};

            vk::FramebufferCreateInfo info{
                vk::FramebufferCreateFlags(),              // Flags
                renderPass.getRenderPass(),                // Render pass
                static_cast<uint32_t>(attachments.size()), // Attachment count
                attachments.data(),                        // Attachments
                extent.width,                              // Width
                extent.height,                             // Height
                1};                                        // Layers

            framebuffers.push_back(device.getDevice().createFramebufferUnique(info));
        }

        return framebuffers;
    }
} // namespace

struct VulkanRenderContext::Internal
{
    const ast::VulkanSwapchain swapchain;
    const ast::VulkanRenderPass renderPass;
    const ast::VulkanImage multiSampleImage;
    const ast::VulkanImageView multiSampleImageView;
    const ast::VulkanImage depthImage;
    const ast::VulkanImageView depthImageView;
    const std::vector<vk::UniqueFramebuffer> framebuffers;

    Internal(const ast::SDLWindow& window,
             const ast::VulkanPhysicalDevice& physicalDevice,
             const ast::VulkanDevice& device,
             const ast::VulkanSurface& surface,
             const ast::VulkanCommandPool& commandPool)
        : swapchain(ast::VulkanSwapchain(window, physicalDevice, device, surface)),
          renderPass(ast::VulkanRenderPass(physicalDevice, device, swapchain)),
          multiSampleImage(::createMultiSampleImage(commandPool, physicalDevice, device, swapchain)),
          multiSampleImageView(::createImageView(device, multiSampleImage, vk::ImageAspectFlagBits::eColor)),
          depthImage(::createDepthImage(commandPool, physicalDevice, device, swapchain)),
          depthImageView(::createImageView(device, depthImage, vk::ImageAspectFlagBits::eDepth)),
          framebuffers(::createFramebuffers(device, swapchain, renderPass, multiSampleImageView, depthImageView)) {}
};

VulkanRenderContext::VulkanRenderContext(const ast::SDLWindow& window,
                                         const ast::VulkanPhysicalDevice& physicalDevice,
                                         const ast::VulkanDevice& device,
                                         const ast::VulkanSurface& surface,
                                         const ast::VulkanCommandPool& commandPool)
    : internal(ast::make_internal_ptr<Internal>(window, physicalDevice, device, surface, commandPool)) {}

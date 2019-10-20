#include "vulkan-surface.hpp"

using ast::VulkanSurface;

namespace
{
    vk::UniqueSurfaceKHR createSurface(const vk::Instance& instance,
                                       const ast::VulkanPhysicalDevice& physicalDevice,
                                       const ast::SDLWindow& window)
    {
        static const std::string logTag{"ast::VulkanSurface::createSurface"};

        VkSurfaceKHR sdlSurface;

        // Ask SDL to create a Vulkan surface from its window.
        if (!SDL_Vulkan_CreateSurface(window.getWindow(), instance, &sdlSurface))
        {
            throw std::runtime_error(logTag + ": SDL could not create a Vulkan surface.");
        }

        // Wrap the result in a Vulkan managed surface object.
        return vk::UniqueSurfaceKHR{sdlSurface, instance};
    }
} // namespace

struct VulkanSurface::Internal
{
    const vk::UniqueSurfaceKHR surface;

    Internal(const vk::Instance& instance,
             const ast::VulkanPhysicalDevice& physicalDevice,
             const ast::SDLWindow& window)
        : surface(::createSurface(instance, physicalDevice, window)) {}
};

VulkanSurface::VulkanSurface(const vk::Instance& instance,
                             const ast::VulkanPhysicalDevice& physicalDevice,
                             const ast::SDLWindow& window)
    : internal(ast::make_internal_ptr<Internal>(instance, physicalDevice, window)) {}

const vk::SurfaceKHR& VulkanSurface::getSurface() const
{
    return *internal->surface;
}

#include "vulkan-physical-device.hpp"
#include "../../core/log.hpp"

using ast::VulkanPhysicalDevice;

namespace
{
    vk::PhysicalDevice createPhysicalDevice(const vk::Instance& instance)
    {
        static const std::string logTag{"ast::VulkanPhysicalDevice::createPhysicalDevice"};

        // Ask Vulkan for all the available physical devices in the current instance.
        std::vector<vk::PhysicalDevice> devices{instance.enumeratePhysicalDevices()};

        // If there are no physical devices available that can drive Vulkan then we
        // will deliberately throw an exception which will ultimately cause our
        // application to fall back to the OpenGL implementation.
        if (devices.empty())
        {
            throw std::runtime_error(logTag + ": No Vulkan physical devices found.");
        }

        // Pick the first device as the default which will be used if no better devices are found.
        vk::PhysicalDevice selectedDevice{devices[0]};

        // Grab the properties of the first device so we can query them for capabilities.
        vk::PhysicalDeviceProperties selectedProperties{selectedDevice.getProperties()};

        // If the device we have selected by default does not have a discrete GPU, then we will
        // try to search through any other physical devices looking for one with a discrete GPU.
        // Some computers will have an 'integrated' GPU as well as a 'discrete' GPU, where the
        // integrated GPU is typically not designed for high end graphics tasks, whereas a discrete
        // GPU is exactly for that purpose.
        if (selectedProperties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu)
        {
            for (size_t i = 1; i < devices.size(); i++)
            {
                vk::PhysicalDevice& nextDevice{devices[i]};
                vk::PhysicalDeviceProperties nextProperties{nextDevice.getProperties()};

                // If we find a device with a discrete GPU, then choose it and we are done.
                if (nextProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
                {
                    selectedDevice = nextDevice;
                    selectedProperties = nextProperties;
                    break;
                }
            }
        }

        // Next we need to make sure that the physical device can support a swapchain.
        bool hasSwapchainSupport{false};
        auto swapchainName{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        // Traverse all the extensions available in the physical device, looking for the
        // presence of the swapchain extension.
        for (const auto& extension : selectedDevice.enumerateDeviceExtensionProperties())
        {
            if (extension.extensionName == swapchainName)
            {
                hasSwapchainSupport = true;
                break;
            }
        }

        // We can't render without swapchain support.
        if (!hasSwapchainSupport)
        {
            throw std::runtime_error(logTag + ": Swapchain support not found.");
        }

        // We should now have selected a physical device, which may or may not have a discrete GPU
        // but will have been selected with a preference to having one.
        ast::log(logTag, "Physical device: " + std::string{selectedProperties.deviceName} + ".");

        return selectedDevice;
    }
} // namespace

struct VulkanPhysicalDevice::Internal
{
    const vk::PhysicalDevice physicalDevice;

    Internal(const vk::Instance& instance)
        : physicalDevice(::createPhysicalDevice(instance)) {}
};

VulkanPhysicalDevice::VulkanPhysicalDevice(const vk::Instance& instance)
    : internal(ast::make_internal_ptr<Internal>(instance)) {}

const vk::PhysicalDevice& VulkanPhysicalDevice::getPhysicalDevice() const
{
    return internal->physicalDevice;
}

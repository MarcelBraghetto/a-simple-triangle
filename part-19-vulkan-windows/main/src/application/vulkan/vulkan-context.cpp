#include "vulkan-context.hpp"
#include "../../core/graphics-wrapper.hpp"
#include "../../core/log.hpp"
#include "vulkan-common.hpp"

using ast::VulkanContext;

namespace
{
    vk::UniqueInstance createInstance()
    {
        vk::ApplicationInfo applicationInfo{
            "A Simple Triangle",      // Application name
            VK_MAKE_VERSION(1, 0, 0), // Application version
            "A Simple Triangle",      // Engine name
            VK_MAKE_VERSION(1, 0, 0), // Engine version
            VK_MAKE_VERSION(1, 0, 0)  // Vulkan API version
        };

        // Find out what the mandatory Vulkan extensions are on the current device,
        // by this stage we would have already determined that the extensions are
        // available via the 'ast::vulkan::isVulkanAvailable()' call in our main engine.
        std::vector<std::string> requiredExtensionNames{
            ast::vulkan::getRequiredVulkanExtensionNames()};

        // Pack the extension names into a data format consumable by Vulkan.
        std::vector<const char*> extensionNames;
        for (const auto& extension : requiredExtensionNames)
        {
            extensionNames.push_back(extension.c_str());
        }

        // Define the info for creating our Vulkan instance.
        vk::InstanceCreateInfo instanceCreateInfo{
            vk::InstanceCreateFlags(),                    // Flags
            &applicationInfo,                             // Application info
            0,                                            // Enabled layer count
            nullptr,                                      // Enabled layer names
            static_cast<uint32_t>(extensionNames.size()), // Enabled extension count
            extensionNames.data()                         // Enabled extension names
        };

        // Build a new Vulkan instance from the configuration.
        return vk::createInstanceUnique(instanceCreateInfo);
    }
} // namespace

struct VulkanContext::Internal
{
    const vk::UniqueInstance instance;

    Internal() : instance(::createInstance())
    {
        ast::log("ast::VulkanContext", "Initialized Vulkan context successfully.");
    }
};

VulkanContext::VulkanContext() : internal(ast::make_internal_ptr<Internal>()) {}

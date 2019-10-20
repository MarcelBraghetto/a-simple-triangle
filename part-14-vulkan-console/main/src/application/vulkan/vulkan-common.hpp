#pragma once

#include <string>
#include <vector>

namespace ast::vulkan
{
    std::vector<std::string> getRequiredVulkanExtensionNames();

    bool isVulkanAvailable();
} // namespace ast::vulkan

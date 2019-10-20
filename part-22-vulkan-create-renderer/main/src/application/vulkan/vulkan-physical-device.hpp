#pragma once

#include "../../core/graphics-wrapper.hpp"
#include "../../core/internal-ptr.hpp"

namespace ast
{
    struct VulkanPhysicalDevice
    {
        VulkanPhysicalDevice(const vk::Instance& instance);

        const vk::PhysicalDevice& getPhysicalDevice() const;

		vk::SampleCountFlagBits getMultiSamplingLevel() const;

		vk::Format getDepthFormat() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

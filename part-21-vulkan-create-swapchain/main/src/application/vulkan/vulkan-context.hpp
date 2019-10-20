#pragma once

#include "../../core/internal-ptr.hpp"

namespace ast
{
    struct VulkanContext
    {
        VulkanContext();

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

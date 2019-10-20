#pragma once

#include "../../core/internal-ptr.hpp"
#include "../application.hpp"

namespace ast
{
    struct VulkanApplication : public ast::Application
    {
        VulkanApplication();

        void update(const float& delta) override;

        void render() override;

        void onWindowResized() override;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

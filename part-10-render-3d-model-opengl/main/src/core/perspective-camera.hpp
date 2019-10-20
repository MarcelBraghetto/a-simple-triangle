#pragma once

#include "../core/glm-wrapper.hpp"
#include "../core/internal-ptr.hpp"

namespace ast
{
    struct PerspectiveCamera
    {
        PerspectiveCamera(const float& width, const float& height);

        const glm::mat4& getProjectionMatrix() const;

        const glm::mat4& getViewMatrix() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

#pragma once

#include "asset-inventory.hpp"
#include "glm-wrapper.hpp"
#include "internal-ptr.hpp"

namespace ast
{
    struct StaticMeshInstance
    {
        StaticMeshInstance(const ast::assets::StaticMesh& staticMesh,
                           const ast::assets::Texture& texture,
                           const glm::vec3& position = glm::vec3{0.0f, 0.0f, 0.0f},
                           const glm::vec3& scale = glm::vec3{1.0f, 1.0f, 1.0f},
                           const glm::vec3& rotationAxis = glm::vec3{0.0f, 1.0f, 0.0f},
                           const float& rotationDegrees = 0.0f);

        void update(const glm::mat4& projectionViewMatrix);

        void rotateBy(const float& degrees);

        ast::assets::StaticMesh getMesh() const;

        ast::assets::Texture getTexture() const;

        glm::mat4 getTransformMatrix() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

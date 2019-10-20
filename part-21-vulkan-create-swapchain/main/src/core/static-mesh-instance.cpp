#include "static-mesh-instance.hpp"

using ast::StaticMeshInstance;

struct StaticMeshInstance::Internal
{
    const ast::assets::StaticMesh mesh;
    const ast::assets::Texture texture;
    const glm::mat4 identity;

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotationAxis;
    float rotationDegrees;
    glm::mat4 transformMatrix;

    Internal(const ast::assets::StaticMesh& mesh,
             const ast::assets::Texture& texture,
             const glm::vec3& position,
             const glm::vec3& scale,
             const glm::vec3& rotationAxis,
             const float& rotationDegrees)
        : mesh(mesh),
          texture(texture),
          identity(glm::mat4{1.0f}),
          position(position),
          scale(scale),
          rotationAxis(rotationAxis),
          rotationDegrees(rotationDegrees),
          transformMatrix(identity) {}

    void update(const glm::mat4& projectionViewMatrix)
    {
        transformMatrix = projectionViewMatrix *
                          glm::translate(identity, position) *
                          glm::rotate(identity, glm::radians(rotationDegrees), rotationAxis) *
                          glm::scale(identity, scale);
    }

    void rotateBy(const float& degrees)
    {
        rotationDegrees += degrees;

        if (rotationDegrees > 360.0f)
        {
            rotationDegrees -= 360.0f;
        }
        else if (rotationDegrees < -360.0f)
        {
            rotationDegrees += 360.0f;
        }
    }
};

StaticMeshInstance::StaticMeshInstance(
    const ast::assets::StaticMesh& staticMesh,
    const ast::assets::Texture& texture,
    const glm::vec3& position,
    const glm::vec3& scale,
    const glm::vec3& rotationAxis,
    const float& rotationDegrees)
    : internal(ast::make_internal_ptr<Internal>(
          staticMesh,
          texture,
          position,
          scale,
          rotationAxis,
          rotationDegrees)) {}

void StaticMeshInstance::update(const glm::mat4& projectionViewMatrix)
{
    internal->update(projectionViewMatrix);
}

void StaticMeshInstance::rotateBy(const float& degrees)
{
    internal->rotateBy(degrees);
}

ast::assets::StaticMesh StaticMeshInstance::getMesh() const
{
    return internal->mesh;
}

ast::assets::Texture StaticMeshInstance::getTexture() const
{
    return internal->texture;
}

glm::mat4 StaticMeshInstance::getTransformMatrix() const
{
    return internal->transformMatrix;
}

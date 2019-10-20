#include "perspective-camera.hpp"

using ast::PerspectiveCamera;

namespace
{
    glm::mat4 createProjectionMatrix(const float& width, const float& height)
    {
        return glm::perspective(glm::radians(60.0f), width / height, 0.01f, 100.0f);
    }

    glm::mat4 createViewMatrix()
    {
        glm::vec3 position{glm::vec3(0.0f, 0.0f, 2.0f)};
        glm::vec3 target{glm::vec3(0.0f, 0.0f, 0.0f)};
        glm::vec3 up{glm::vec3(0.0f, 1.0f, 0.0f)};

        return glm::lookAt(position, target, up);
    }

} // namespace

struct PerspectiveCamera::Internal
{
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    Internal(const float& width, const float& height)
        : projectionMatrix(::createProjectionMatrix(width, height)),
          viewMatrix(::createViewMatrix()) {}
};

PerspectiveCamera::PerspectiveCamera(const float& width, const float& height)
    : internal(ast::make_internal_ptr<Internal>(width, height)) {}

const glm::mat4& PerspectiveCamera::getProjectionMatrix() const
{
    return internal->projectionMatrix;
}

const glm::mat4& PerspectiveCamera::getViewMatrix() const
{
    return internal->viewMatrix;
}

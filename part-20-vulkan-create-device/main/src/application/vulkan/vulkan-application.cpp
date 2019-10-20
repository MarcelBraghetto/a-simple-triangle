#include "vulkan-application.hpp"
#include "../../core/graphics-wrapper.hpp"
#include "vulkan-context.hpp"

using ast::VulkanApplication;

struct VulkanApplication::Internal
{
    const ast::VulkanContext context;

    Internal() : context(ast::VulkanContext()) {}

    void update(const float& delta) {}

    void render() {}
};

VulkanApplication::VulkanApplication() : internal(ast::make_internal_ptr<Internal>()) {}

void VulkanApplication::update(const float& delta)
{
    internal->update(delta);
}

void VulkanApplication::render()
{
    internal->render();
}

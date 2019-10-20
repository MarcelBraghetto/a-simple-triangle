#include "vulkan-application.hpp"
#include "../../core/graphics-wrapper.hpp"
#include "../../core/sdl-wrapper.hpp"
#include "vulkan-context.hpp"

using ast::VulkanApplication;

struct VulkanApplication::Internal
{
    const ast::VulkanContext context;
    SDL_Window* window;

    Internal() : context(ast::VulkanContext()),
                 window(ast::sdl::createWindow(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI)) {}

    void update(const float& delta) {}

    void render() {}

    ~Internal()
    {
        if (window)
        {
            SDL_DestroyWindow(window);
        }
    }
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

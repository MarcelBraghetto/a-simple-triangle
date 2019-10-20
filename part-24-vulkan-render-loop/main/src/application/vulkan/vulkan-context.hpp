#pragma once

#include "../../core/internal-ptr.hpp"
#include "../../core/renderer.hpp"
#include "vulkan-asset-manager.hpp"

namespace ast
{
    struct VulkanContext : public ast::Renderer
    {
        VulkanContext(std::shared_ptr<ast::VulkanAssetManager> assetManager);

        bool renderBegin();

        void render(
            const ast::assets::Pipeline& pipeline,
            const std::vector<ast::StaticMeshInstance>& staticMeshInstances) override;

        void renderEnd();

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

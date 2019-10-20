#pragma once

#include "../../core/asset-manifest.hpp"
#include "../../core/internal-ptr.hpp"
#include "../../core/renderer.hpp"

namespace ast
{
    struct VulkanContext : public ast::Renderer
    {
        VulkanContext();

        void loadAssetManifest(const ast::AssetManifest& assetManifest);

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

#pragma once

#include "../../core/asset-manager.hpp"
#include "../../core/internal-ptr.hpp"

namespace ast
{
    struct VulkanAssetManager : public ast::AssetManager
    {
        VulkanAssetManager();

        void loadPipelines(const std::vector<ast::assets::Pipeline>& pipelines) override;

        void loadStaticMeshes(const std::vector<ast::assets::StaticMesh>& staticMeshes) override;

        void loadTextures(const std::vector<ast::assets::Texture>& textures) override;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

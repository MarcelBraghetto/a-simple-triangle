#pragma once

#include "asset-inventory.hpp"
#include <vector>

namespace ast
{
    struct AssetManager
    {
        virtual void loadPipelines(const std::vector<ast::assets::Pipeline>& pipelines) = 0;

        virtual void loadStaticMeshes(const std::vector<ast::assets::StaticMesh>& staticMeshes) = 0;

        virtual void loadTextures(const std::vector<ast::assets::Texture>& textures) = 0;
    };
} // namespace ast

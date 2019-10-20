#pragma once

#include "asset-inventory.hpp"
#include <vector>

namespace ast
{
    struct AssetManifest
    {
        const std::vector<ast::assets::Pipeline> pipelines;

        const std::vector<ast::assets::StaticMesh> staticMeshes;

        const std::vector<ast::assets::Texture> textures;
    };
} // namespace ast

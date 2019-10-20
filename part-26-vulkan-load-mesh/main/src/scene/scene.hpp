#pragma once

#include "../core/asset-manifest.hpp"
#include "../core/renderer.hpp"

namespace ast
{
    struct Scene
    {
        Scene() = default;

        virtual ~Scene() = default;

        virtual ast::AssetManifest getAssetManifest() = 0;

        virtual void prepare() = 0;

        virtual void update(const float& delta) = 0;

        virtual void render(ast::Renderer& renderer) = 0;
    };
} // namespace ast

#pragma once

#include "../core/asset-manager.hpp"
#include "../core/renderer.hpp"

namespace ast
{
    struct Scene
    {
        Scene() = default;

        virtual ~Scene() = default;

        virtual void prepare(ast::AssetManager& assetManager) = 0;

        virtual void update(const float& delta) = 0;

        virtual void render(ast::Renderer& renderer) = 0;
    };
} // namespace ast

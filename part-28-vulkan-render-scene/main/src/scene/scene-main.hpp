#pragma once

#include "../core/internal-ptr.hpp"
#include "scene.hpp"

namespace ast
{
    struct SceneMain : public ast::Scene
    {
        SceneMain(const float& screenWidth, const float& screenHeight);

        ast::AssetManifest getAssetManifest() override;

        void prepare() override;

        void update(const float& delta) override;

        void render(ast::Renderer& renderer) override;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

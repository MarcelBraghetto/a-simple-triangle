#pragma once

#include "../core/internal-ptr.hpp"
#include "../core/window-size.hpp"
#include "scene.hpp"

namespace ast
{
    struct SceneMain : public ast::Scene
    {
        SceneMain(const ast::WindowSize& frameSize);

        ast::AssetManifest getAssetManifest() override;

        void prepare() override;

        void update(const float& delta) override;

        void render(ast::Renderer& renderer) override;

        void onWindowResized(const ast::WindowSize& size) override;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

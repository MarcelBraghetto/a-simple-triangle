#pragma once

#include "../../core/internal-ptr.hpp"
#include "../../core/renderer.hpp"
#include "opengl-asset-manager.hpp"
#include <memory>

namespace ast
{
    struct OpenGLRenderer : public ast::Renderer
    {
        OpenGLRenderer(std::shared_ptr<ast::OpenGLAssetManager> assetManager);

        void render(
            const ast::assets::Pipeline& pipeline,
            const std::vector<ast::StaticMeshInstance>& staticMeshInstances) override;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

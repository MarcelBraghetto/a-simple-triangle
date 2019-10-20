#pragma once

#include "../../core/asset-manager.hpp"
#include "../../core/internal-ptr.hpp"
#include "opengl-mesh.hpp"
#include "opengl-pipeline.hpp"
#include "opengl-texture.hpp"

namespace ast
{
    struct OpenGLAssetManager : public ast::AssetManager
    {
        OpenGLAssetManager();

        void loadPipelines(const std::vector<ast::assets::Pipeline>& pipelines) override;

        void loadStaticMeshes(const std::vector<ast::assets::StaticMesh>& staticMeshes) override;

        void loadTextures(const std::vector<ast::assets::Texture>& textures) override;

        const ast::OpenGLPipeline& getPipeline(const ast::assets::Pipeline& pipeline) const;

        const ast::OpenGLMesh& getStaticMesh(const ast::assets::StaticMesh& staticMesh) const;

        const ast::OpenGLTexture& getTexture(const ast::assets::Texture& texture) const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

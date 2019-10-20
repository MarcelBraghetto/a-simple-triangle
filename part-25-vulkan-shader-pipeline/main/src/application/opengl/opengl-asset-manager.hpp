#pragma once

#include "../../core/asset-manifest.hpp"
#include "../../core/internal-ptr.hpp"
#include "opengl-mesh.hpp"
#include "opengl-pipeline.hpp"
#include "opengl-texture.hpp"

namespace ast
{
    struct OpenGLAssetManager
    {
        OpenGLAssetManager();

        void loadAssetManifest(const ast::AssetManifest& assetManifest);

        const ast::OpenGLPipeline& getPipeline(const ast::assets::Pipeline& pipeline) const;

        const ast::OpenGLMesh& getStaticMesh(const ast::assets::StaticMesh& staticMesh) const;

        const ast::OpenGLTexture& getTexture(const ast::assets::Texture& texture) const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

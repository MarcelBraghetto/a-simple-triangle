#pragma once

#include "../../core/internal-ptr.hpp"
#include "../../core/static-mesh-instance.hpp"
#include <string>
#include <vector>

namespace ast
{
    struct OpenGLAssetManager;

    struct OpenGLPipeline
    {
        OpenGLPipeline(const std::string& shaderName);

        void render(
            const ast::OpenGLAssetManager& assetManager,
            const std::vector<ast::StaticMeshInstance>& staticMeshInstances) const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

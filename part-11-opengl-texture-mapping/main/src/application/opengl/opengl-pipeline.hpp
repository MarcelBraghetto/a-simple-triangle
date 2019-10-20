#pragma once

#include "../../core/glm-wrapper.hpp"
#include "../../core/graphics-wrapper.hpp"
#include "../../core/internal-ptr.hpp"
#include "opengl-mesh.hpp"
#include "opengl-texture.hpp"
#include <string>

namespace ast
{
    struct OpenGLPipeline
    {
        OpenGLPipeline(const std::string& shaderName);

        void render(const ast::OpenGLMesh& mesh, const ast::OpenGLTexture& texture, const glm::mat4& mvp) const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

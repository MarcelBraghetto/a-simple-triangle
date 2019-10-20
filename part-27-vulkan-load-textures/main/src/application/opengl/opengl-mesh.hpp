#pragma once

#include "../../core/graphics-wrapper.hpp"
#include "../../core/internal-ptr.hpp"
#include "../../core/mesh.hpp"

namespace ast
{
    struct OpenGLMesh
    {
        OpenGLMesh(const ast::Mesh& mesh);

        const GLuint& getVertexBufferId() const;

        const GLuint& getIndexBufferId() const;

        const uint32_t& getNumIndices() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

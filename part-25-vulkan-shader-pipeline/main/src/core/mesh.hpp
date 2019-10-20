#pragma once

#include "internal-ptr.hpp"
#include "vertex.hpp"
#include <vector>

namespace ast
{
    struct Mesh
    {
        Mesh(const std::vector<ast::Vertex>& vertices, const std::vector<uint32_t>& indices);

        const std::vector<ast::Vertex>& getVertices() const;

        const std::vector<uint32_t>& getIndices() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

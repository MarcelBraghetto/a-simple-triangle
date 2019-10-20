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

        const uint32_t& getNumVertices() const;

        const uint32_t& getNumIndices() const;

    private:
        struct Internal;
        ast::internal_ptr<Internal> internal;
    };
} // namespace ast

#include "mesh.hpp"

using ast::Mesh;

struct Mesh::Internal
{
    const std::vector<ast::Vertex> vertices;
    const std::vector<uint32_t> indices;

    Internal(const std::vector<ast::Vertex>& vertices, const std::vector<uint32_t>& indices)
        : vertices(vertices), indices(indices) {}
};

Mesh::Mesh(const std::vector<ast::Vertex>& vertices, const std::vector<uint32_t>& indices)
    : internal(ast::make_internal_ptr<Internal>(vertices, indices)) {}

const std::vector<ast::Vertex>& Mesh::getVertices() const
{
    return internal->vertices;
}

const std::vector<uint32_t>& Mesh::getIndices() const
{
    return internal->indices;
}

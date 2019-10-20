#include "mesh.hpp"

using ast::Mesh;

struct Mesh::Internal
{
    const std::vector<ast::Vertex> vertices;
    const uint32_t numVertices;
    const std::vector<uint32_t> indices;
    const uint32_t numIndices;

    Internal(const std::vector<ast::Vertex>& vertices, const std::vector<uint32_t>& indices)
        : vertices(vertices),
          numVertices(static_cast<uint32_t>(vertices.size())),
          indices(indices),
          numIndices(static_cast<uint32_t>(indices.size())) {}
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

const uint32_t& Mesh::getNumVertices() const
{
    return internal->numVertices;
}

const uint32_t& Mesh::getNumIndices() const
{
    return internal->numIndices;
}

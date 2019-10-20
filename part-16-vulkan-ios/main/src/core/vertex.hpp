#pragma once

#include "glm-wrapper.hpp"

namespace ast
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 texCoord;

        bool operator==(const ast::Vertex& other) const;
    };
} // namespace ast

namespace std
{
    template <>
    struct hash<ast::Vertex>
    {
        size_t operator()(const ast::Vertex& vertex) const
        {
            return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec2>()(vertex.texCoord) << 1)) >> 1);
        }
    };
} // namespace std

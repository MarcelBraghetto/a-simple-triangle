#include "vertex.hpp"

using ast::Vertex;

bool Vertex::operator==(const Vertex& other) const
{
    return position == other.position && texCoord == other.texCoord;
}

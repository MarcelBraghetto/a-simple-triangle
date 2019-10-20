#pragma once

#include "bitmap.hpp"
#include "mesh.hpp"
#include <string>

namespace ast::assets
{
    std::string loadTextFile(const std::string& path);

    ast::Mesh loadOBJFile(const std::string& path);

    ast::Bitmap loadBitmap(const std::string& path);
} // namespace ast::assets

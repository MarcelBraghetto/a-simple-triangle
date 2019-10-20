#pragma once

#include "asset-inventory.hpp"
#include "static-mesh-instance.hpp"
#include <vector>

namespace ast
{
    struct Renderer
    {
        virtual void render(
            const ast::assets::Pipeline& pipeline,
            const std::vector<ast::StaticMeshInstance>& staticMeshInstances) = 0;
    };
} // namespace ast

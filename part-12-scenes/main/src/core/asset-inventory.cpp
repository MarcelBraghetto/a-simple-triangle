#include "asset-inventory.hpp"

std::string ast::assets::resolvePipelinePath(const ast::assets::Pipeline& pipeline)
{
    switch (pipeline)
    {
        case ast::assets::Pipeline::Default:
            return "default";
    }
}

std::string ast::assets::resolveStaticMeshPath(const ast::assets::StaticMesh& staticMesh)
{
    switch (staticMesh)
    {
        case ast::assets::StaticMesh::Crate:
            return "assets/models/crate.obj";
        case ast::assets::StaticMesh::Torus:
            return "assets/models/torus.obj";

    }
}

std::string ast::assets::resolveTexturePath(const ast::assets::Texture& texture)
{
    switch (texture)
    {
        case ast::assets::Texture::Crate:
            return "assets/textures/crate.png";
        case ast::assets::Texture::RedCrossHatch:
            return "assets/textures/red_cross_hatch.png";
    }
}

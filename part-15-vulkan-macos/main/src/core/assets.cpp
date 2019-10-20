#define TINYOBJLOADER_IMPLEMENTATION

#include "assets.hpp"
#include "sdl-wrapper.hpp"
#include "vertex.hpp"
#include <SDL_image.h>
#include <sstream>
#include <tiny_obj_loader.h>
#include <unordered_map>
#include <vector>

std::string ast::assets::loadTextFile(const std::string& path)
{
    // Open a file operation handle to the asset file.
    SDL_RWops* file{SDL_RWFromFile(path.c_str(), "r")};

    // Determine how big the file is.
    size_t fileLength{static_cast<size_t>(SDL_RWsize(file))};

    // Ask SDL to load the content of the file into a data pointer.
    void* data{SDL_LoadFile_RW(file, nullptr, 1)};

    // Construct a new string based on the data pointer.
    std::string result(static_cast<char*>(data), fileLength);

    // Free up the original SDL managed data pointer.
    SDL_free(data);

    // Hand back the resulting string which is the content of the file.
    return result;
}

ast::Mesh ast::assets::loadOBJFile(const std::string& path)
{
    // Load the .obj file into a string and wrap it as a stream.
    std::istringstream sourceStream(ast::assets::loadTextFile(path));

    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning;
    std::string error;

    // Ask tinyobj to parse the string loaded from our file.
    if (!tinyobj::LoadObj(
            &attributes,
            &shapes,
            &materials,
            &warning,
            &error,
            &sourceStream))
    {
        throw std::runtime_error("ast::assets::loadOBJFile: Error: " + warning + error);
    }

    std::vector<ast::Vertex> vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<ast::Vertex, uint32_t> uniqueVertices;

    // Loop through all the shapes that there found.
    for (const auto& shape : shapes)
    {
        // For each shape, loop through its indices.
        for (const auto& index : shape.mesh.indices)
        {
            // Construct a new (x, y, z) position for the current mesh index.
            glm::vec3 position{
                attributes.vertices[3 * index.vertex_index + 0],
                attributes.vertices[3 * index.vertex_index + 1],
                attributes.vertices[3 * index.vertex_index + 2]};

            // Construct a new (u, v) texture coordinate for the current mesh index.
            glm::vec2 texCoord{
                attributes.texcoords[2 * index.texcoord_index + 0],
                1.0f - attributes.texcoords[2 * index.texcoord_index + 1]};

            // Construct a vertex with the extracted data.
            ast::Vertex vertex{position, texCoord};

            // This will help deduplicate vertices - we maintain a hash map where a
            // vertex is used as a unique key with its value being which index can
            // be used to locate the vertex. The vertex is only added if it has not
            // been added before.
            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }

    return ast::Mesh{vertices, indices};
}

ast::Bitmap ast::assets::loadBitmap(const std::string& path)
{
    SDL_RWops* file{SDL_RWFromFile(path.c_str(), "rb")};
    SDL_Surface* source{IMG_Load_RW(file, 1)};
    SDL_Rect imageFrame{0, 0, source->w, source->h};

    uint32_t redMask;
    uint32_t greenMask;
    uint32_t blueMask;
    uint32_t alphaMask;

#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    redMask = 0xff000000;
    greenMask = 0x00ff0000;
    blueMask = 0x0000ff00;
    alphaMask = 0x000000ff;
#else
    redMask = 0x000000ff;
    greenMask = 0x0000ff00;
    blueMask = 0x00ff0000;
    alphaMask = 0xff000000;
#endif

    SDL_Surface* target{SDL_CreateRGBSurface(
        0,
        imageFrame.w, imageFrame.h,
        32,
        redMask, greenMask, blueMask, alphaMask)};

    SDL_BlitSurface(source, &imageFrame, target, &imageFrame);

    SDL_FreeSurface(source);

    return ast::Bitmap(target);
}

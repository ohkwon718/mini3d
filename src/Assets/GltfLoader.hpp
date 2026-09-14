#pragma once

#include "Geometry/Mesh.hpp"
#include "Scene/Material.hpp"

#include <cstddef>
#include <filesystem>

struct GltfSummary
{
    std::size_t meshes;
    std::size_t materials;
    std::size_t images;
    std::size_t nodes;
};

struct LoadedPrimitive
{
    Mesh mesh;
    Material material;
};

GltfSummary inspectGltf(
    const std::filesystem::path& path
);

LoadedPrimitive loadFirstPrimitive(
    const std::filesystem::path& path
);
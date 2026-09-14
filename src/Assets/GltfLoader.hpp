#pragma once

#include "Geometry/Mesh.hpp"

#include <cstddef>
#include <filesystem>

struct GltfSummary
{
    std::size_t meshes;
    std::size_t materials;
    std::size_t images;
    std::size_t nodes;
};

GltfSummary inspectGltf(
    const std::filesystem::path& path
);

Mesh loadFirstMesh(
    const std::filesystem::path& path
);
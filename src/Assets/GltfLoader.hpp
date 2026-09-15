#pragma once

#include "Geometry/Mesh.hpp"
#include "Transform/Transform.hpp"
#include "Scene/Material.hpp"

#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

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

struct LoadedObject
{
    std::shared_ptr<const Mesh> mesh;
    Material material;
    Transform transform;
    std::string name;
};



GltfSummary inspectGltf(
    const std::filesystem::path& path
);

std::vector<LoadedObject>  loadGltfObjects(
    const std::filesystem::path& path
);
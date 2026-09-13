#include "GltfLoader.hpp"

#include <fastgltf/core.hpp>
#include <fastgltf/types.hpp>

#include <stdexcept>

GltfSummary inspectGltf(
    const std::filesystem::path& path)
{
    fastgltf::Parser parser;

    auto data = fastgltf::GltfDataBuffer::FromPath(path);

    if (data.error() != fastgltf::Error::None) {
        throw std::runtime_error(
            "Failed to open glTF file"
        );
    }

    auto asset = parser.loadGltf(
        data.get(),
        path.parent_path(),
        fastgltf::Options::LoadExternalBuffers |
        fastgltf::Options::LoadExternalImages
    );

    if (asset.error() != fastgltf::Error::None) {
        throw std::runtime_error(
            "Failed to parse glTF"
        );
    }

    return {
        asset->meshes.size(),
        asset->materials.size(),
        asset->images.size(),
        asset->nodes.size()
    };
}
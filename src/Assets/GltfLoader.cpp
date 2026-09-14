#include "GltfLoader.hpp"

#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/types.hpp>

#include <Eigen/Dense>

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace
{

fastgltf::Asset loadAsset(
    const std::filesystem::path& path)
{
    fastgltf::Parser parser;

    auto data =
        fastgltf::GltfDataBuffer::FromPath(path);

    if (data.error() != fastgltf::Error::None) {
        throw std::runtime_error(
            "Failed to open glTF: " +
            std::string(
                fastgltf::getErrorMessage(data.error())
            )
        );
    }

    constexpr auto options =
        fastgltf::Options::LoadExternalBuffers |
        fastgltf::Options::LoadExternalImages |
        fastgltf::Options::LoadGLBBuffers |
        fastgltf::Options::GenerateMeshIndices;

    auto result = parser.loadGltf(
        data.get(),
        path.parent_path(),
        options
    );

    if (result.error() != fastgltf::Error::None) {
        throw std::runtime_error(
            "Failed to parse glTF: " +
            std::string(
                fastgltf::getErrorMessage(result.error())
            )
        );
    }

    return std::move(result.get());
}

} // namespace


GltfSummary inspectGltf(
    const std::filesystem::path& path)
{
    auto asset = loadAsset(path);

    return {
        asset.meshes.size(),
        asset.materials.size(),
        asset.images.size(),
        asset.nodes.size()
    };
}


Mesh loadFirstMesh(
    const std::filesystem::path& path)
{
    auto asset = loadAsset(path);

    if (asset.meshes.empty()) {
        throw std::runtime_error(
            "glTF contains no meshes"
        );
    }

    const auto& gltfMesh =
        asset.meshes.front();

    if (gltfMesh.primitives.empty()) {
        throw std::runtime_error(
            "glTF mesh contains no primitives"
        );
    }

    const auto& primitive =
        gltfMesh.primitives.front();

    if (primitive.type !=
        fastgltf::PrimitiveType::Triangles) {
        throw std::runtime_error(
            "Only triangle primitives are supported"
        );
    }

    const auto* positionIt =
        primitive.findAttribute("POSITION");

    const auto* normalIt =
        primitive.findAttribute("NORMAL");

    const auto* texCoordIt =
        primitive.findAttribute("TEXCOORD_0");

    if (positionIt == primitive.attributes.end()) {
        throw std::runtime_error(
            "Primitive has no POSITION attribute"
        );
    }

    if (normalIt == primitive.attributes.end()) {
        throw std::runtime_error(
            "Primitive has no NORMAL attribute"
        );
    }

    if (texCoordIt == primitive.attributes.end()) {
        throw std::runtime_error(
            "Primitive has no TEXCOORD_0 attribute"
        );
    }

    if (!primitive.indicesAccessor) {
        throw std::runtime_error(
            "Primitive has no index accessor"
        );
    }

    const auto& positionAccessor =
        asset.accessors.at(
            positionIt->accessorIndex
        );

    const auto& normalAccessor =
        asset.accessors.at(
            normalIt->accessorIndex
        );

    const auto& texCoordAccessor =
        asset.accessors.at(
            texCoordIt->accessorIndex
        );

    const auto& indexAccessor =
        asset.accessors.at(
            *primitive.indicesAccessor
        );

    if (normalAccessor.count !=
            positionAccessor.count ||
        texCoordAccessor.count !=
            positionAccessor.count) {
        throw std::runtime_error(
            "Vertex attribute counts do not match"
        );
    }

    std::vector<Vertex> vertices(
        positionAccessor.count
    );

    fastgltf::iterateAccessorWithIndex<
        fastgltf::math::fvec3>(
        asset,
        positionAccessor,
        [&vertices](fastgltf::math::fvec3 position,
            std::size_t index)
        {
            vertices[index].position =
                Eigen::Vector3f(
                    position.x(),
                    position.y(),
                    position.z()
                );
        }
    );

    fastgltf::iterateAccessorWithIndex<
        fastgltf::math::fvec3>(
        asset,
        normalAccessor,
        [&vertices](fastgltf::math::fvec3 normal,
            std::size_t index)
        {
            vertices[index].normal =
                Eigen::Vector3f(
                    normal.x(),
                    normal.y(),
                    normal.z()
                );
        }
    );

    fastgltf::iterateAccessorWithIndex<
        fastgltf::math::fvec2>(
        asset,
        texCoordAccessor,
        [&vertices](fastgltf::math::fvec2 uv,
            std::size_t index)
        {
            vertices[index].texCoord =
                Eigen::Vector2f(
                    uv.x(),
                    uv.y()
                );
        }
    );

    std::vector<std::uint32_t> indices(
        indexAccessor.count
    );

    fastgltf::iterateAccessorWithIndex<
        std::uint32_t>(
        asset,
        indexAccessor,
        [&indices](std::uint32_t value,
            std::size_t index)
        {
            indices[index] = value;
        }
    );

    return Mesh(
        std::move(vertices),
        std::move(indices)
    );
}
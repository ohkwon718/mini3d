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
#include <span>
#include <type_traits>

#include <core/ImageLoader.hpp>

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

std::span<const std::byte> bytesFromDataSource(
    const fastgltf::Asset& asset,
    const fastgltf::DataSource& data)
{
    return std::visit(
        [&asset](const auto& source)
            -> std::span<const std::byte>
        {
            using T =
                std::decay_t<decltype(source)>;

            if constexpr (
                std::is_same_v<
                    T,
                    fastgltf::sources::Array>)
            {
                return {
                    source.bytes.data(),
                    source.bytes.size()
                };
            }
            else if constexpr (
                std::is_same_v<
                    T,
                    fastgltf::sources::Vector>)
            {
                return {
                    source.bytes.data(),
                    source.bytes.size()
                };
            }
            else if constexpr (
                std::is_same_v<
                    T,
                    fastgltf::sources::ByteView>)
            {
                return {
                    source.bytes.data(),
                    source.bytes.size()
                };
            }
            else if constexpr (
                std::is_same_v<
                    T,
                    fastgltf::sources::BufferView>)
            {
                const auto& bufferView =
                    asset.bufferViews.at(
                        source.bufferViewIndex
                    );

                const auto& buffer =
                    asset.buffers.at(
                        bufferView.bufferIndex
                    );

                auto bufferBytes =
                    bytesFromDataSource(
                        asset,
                        buffer.data
                    );

                if (bufferView.byteOffset >
                        bufferBytes.size() ||
                    bufferView.byteLength >
                        bufferBytes.size() -
                            bufferView.byteOffset)
                {
                    throw std::runtime_error(
                        "Invalid glTF image buffer view"
                    );
                }

                return bufferBytes.subspan(
                    bufferView.byteOffset,
                    bufferView.byteLength
                );
            }
            else
            {
                throw std::runtime_error(
                    "Unsupported glTF image data source"
                );
            }
        },
        data
    );
}

Transform transformFromGltfMatrix(
    const fastgltf::math::fmat4x4& matrix)
{
    fastgltf::math::fvec3 scale;
    fastgltf::math::fquat rotation;
    fastgltf::math::fvec3 translation;

    fastgltf::math::decomposeTransformMatrix(
        matrix,
        scale,
        rotation,
        translation
    );

    Transform transform;

    transform.setTranslation({
        translation.x(),
        translation.y(),
        translation.z()
    });

    transform.setRotation(
        Eigen::Quaternionf(
            rotation.w(),
            rotation.x(),
            rotation.y(),
            rotation.z()
        )
    );

    transform.setScale({
        scale.x(),
        scale.y(),
        scale.z()
    });

    return transform;
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

std::vector<LoadedObject> loadGltfObjects(const std::filesystem::path& path)
{
    auto asset = loadAsset(path);

    if (asset.meshes.empty()) {
        throw std::runtime_error(
            "glTF contains no meshes"
        );
    }
        
    if (asset.meshes.empty()) {
        throw std::runtime_error("glTF contains no meshes");
    }

    std::vector<LoadedObject>  loaded;

    if (asset.scenes.empty()) {
        throw std::runtime_error(
            "glTF contains no scenes"
        );
    }

    const std::size_t sceneIndex = asset.defaultScene.value_or(0);

    fastgltf::iterateSceneNodes(
        asset,
        sceneIndex,
        fastgltf::math::fmat4x4(),
        [&](fastgltf::Node& node,
            fastgltf::math::fmat4x4 matrix)
        {
            if (!node.meshIndex) {
                return;
            }

            const auto& gltfMesh =
                asset.meshes.at(*node.meshIndex);

            for (std::size_t primitiveIndex = 0;
                primitiveIndex < gltfMesh.primitives.size();
                ++primitiveIndex)
            {
                const auto& primitive =
                    gltfMesh.primitives[primitiveIndex];                

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

                const auto& indexAccessor =
                    asset.accessors.at(
                        *primitive.indicesAccessor
                    );

                if (normalAccessor.count != positionAccessor.count) {
                    throw std::runtime_error(
                        "Normal count does not match position count"
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

                if (texCoordIt != primitive.attributes.end()) {
                    const auto& texCoordAccessor =
                        asset.accessors.at(
                            texCoordIt->accessorIndex
                        );

                    if (texCoordAccessor.count != positionAccessor.count) {
                        throw std::runtime_error(
                            "Texture coordinate count does not match position count"
                        );
                    }

                    fastgltf::iterateAccessorWithIndex<
                        fastgltf::math::fvec2>(
                        asset,
                        texCoordAccessor,
                        [&vertices](
                            fastgltf::math::fvec2 uv,
                            std::size_t index)
                        {
                            vertices[index].texCoord = {
                                uv.x(),
                                uv.y()
                            };
                        }
                    );
                }

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

                Mesh mesh(
                    std::move(vertices),
                    std::move(indices)
                );

                Material material;

                if (primitive.materialIndex) {
                    const auto& gltfMaterial =
                        asset.materials.at(*primitive.materialIndex);

                    const auto& baseColor =
                        gltfMaterial.pbrData.baseColorFactor;

                    material.baseColor = {
                        baseColor[0],
                        baseColor[1],
                        baseColor[2]
                    };

                    if (gltfMaterial.pbrData.baseColorTexture) {
                        const auto& textureInfo =
                            *gltfMaterial.pbrData.baseColorTexture;

                        if (textureInfo.texCoordIndex != 0) {
                            throw std::runtime_error(
                                "Only TEXCOORD_0 is currently supported"
                            );
                        }

                        const auto& texture =
                            asset.textures.at(textureInfo.textureIndex);

                        if (texture.imageIndex) {
                            const auto& gltfImage =
                                asset.images.at(*texture.imageIndex);

                        auto encodedBytes =
                            bytesFromDataSource(
                                asset,
                                gltfImage.data
                            );

                        material.baseColorImage =
                            std::make_shared<const Image>(
                                loadImage(encodedBytes)
                            );
                        }
                    }
                }
                if (material.baseColorImage &&
                    texCoordIt == primitive.attributes.end()) {
                    throw std::runtime_error(
                        "Textured primitive has no TEXCOORD_0 attribute"
                    );
                }
                
                Transform transform = transformFromGltfMatrix(matrix);

                std::string name(
                    node.name.data(),
                    node.name.size()
                );

                if (name.empty()) {
                    name = std::string(
                        gltfMesh.name.data(),
                        gltfMesh.name.size()
                    );
                }

                if (name.empty()) {
                    name = "gltf_object";
                }

                if (gltfMesh.primitives.size() > 1) {
                    name += "_primitive_" +
                        std::to_string(primitiveIndex);
                }

                loaded.push_back({
                    std::make_shared<const Mesh>(std::move(mesh)), 
                    std::move(material), 
                    std::move(transform), 
                    std::move(name)
                });

            }
        }
    );

    
    return loaded;

}
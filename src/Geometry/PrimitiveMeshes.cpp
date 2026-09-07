#include "PrimitiveMeshes.hpp"

Mesh createCubeMesh(float halfExtent)
{
    const float h = halfExtent;
    std::vector<Vertex> vertices = {
        // Front (+Z)
        {Eigen::Vector3f(-h, -h,  h), Eigen::Vector3f( 0.0f,  0.0f,  1.0f)},
        {Eigen::Vector3f( h, -h,  h), Eigen::Vector3f( 0.0f,  0.0f,  1.0f)},
        {Eigen::Vector3f( h,  h,  h), Eigen::Vector3f( 0.0f,  0.0f,  1.0f)},
        {Eigen::Vector3f(-h,  h,  h), Eigen::Vector3f( 0.0f,  0.0f,  1.0f)},

        // Back (-Z)
        {Eigen::Vector3f( h, -h, -h), Eigen::Vector3f( 0.0f,  0.0f, -1.0f)},
        {Eigen::Vector3f(-h, -h, -h), Eigen::Vector3f( 0.0f,  0.0f, -1.0f)},
        {Eigen::Vector3f(-h,  h, -h), Eigen::Vector3f( 0.0f,  0.0f, -1.0f)},
        {Eigen::Vector3f( h,  h, -h), Eigen::Vector3f( 0.0f,  0.0f, -1.0f)},

        // Left (-X)
        {Eigen::Vector3f(-h, -h, -h), Eigen::Vector3f(-1.0f,  0.0f,  0.0f)},
        {Eigen::Vector3f(-h, -h,  h), Eigen::Vector3f(-1.0f,  0.0f,  0.0f)},
        {Eigen::Vector3f(-h,  h,  h), Eigen::Vector3f(-1.0f,  0.0f,  0.0f)},
        {Eigen::Vector3f(-h,  h, -h), Eigen::Vector3f(-1.0f,  0.0f,  0.0f)},

        // Right (+X)
        {Eigen::Vector3f( h, -h,  h), Eigen::Vector3f( 1.0f,  0.0f,  0.0f)},
        {Eigen::Vector3f( h, -h, -h), Eigen::Vector3f( 1.0f,  0.0f,  0.0f)},
        {Eigen::Vector3f( h,  h, -h), Eigen::Vector3f( 1.0f,  0.0f,  0.0f)},
        {Eigen::Vector3f( h,  h,  h), Eigen::Vector3f( 1.0f,  0.0f,  0.0f)},

        // Top (+Y)
        {Eigen::Vector3f(-h,  h,  h), Eigen::Vector3f( 0.0f,  1.0f,  0.0f)},
        {Eigen::Vector3f( h,  h,  h), Eigen::Vector3f( 0.0f,  1.0f,  0.0f)},
        {Eigen::Vector3f( h,  h, -h), Eigen::Vector3f( 0.0f,  1.0f,  0.0f)},
        {Eigen::Vector3f(-h,  h, -h), Eigen::Vector3f( 0.0f,  1.0f,  0.0f)},

        // Bottom (-Y)
        {Eigen::Vector3f(-h, -h, -h), Eigen::Vector3f( 0.0f, -1.0f,  0.0f)},
        {Eigen::Vector3f( h, -h, -h), Eigen::Vector3f( 0.0f, -1.0f,  0.0f)},
        {Eigen::Vector3f( h, -h,  h), Eigen::Vector3f( 0.0f, -1.0f,  0.0f)},
        {Eigen::Vector3f(-h, -h,  h), Eigen::Vector3f( 0.0f, -1.0f,  0.0f)},
    };

    std::vector<std::uint32_t> indices = {
        0,  1,  2,   2,  3,  0,   // Front
        4,  5,  6,   6,  7,  4,   // Back
        8,  9, 10,  10, 11,  8,   // Left
        12, 13, 14,  14, 15, 12,   // Right
        16, 17, 18,  18, 19, 16,   // Top
        20, 21, 22,  22, 23, 20    // Bottom
    };

    return Mesh(std::move(vertices), std::move(indices));   
}

Mesh createUvSphereMesh(float radius,
                        std::uint32_t latitudeSegments,
                        std::uint32_t longitudeSegments)
{


}
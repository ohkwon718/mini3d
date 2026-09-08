#include "PrimitiveMeshes.hpp"
#include <stdexcept>
#include <numbers>
#include <cmath>

Mesh createCubeMesh(float halfExtent)
{
    if (halfExtent <= 0.0f) {
        throw std::invalid_argument("halfExtent must be positive");
    }
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
    if (radius <= 0.0f) {
        throw std::invalid_argument("radius must be positive");
    }
    if (latitudeSegments < 2) {
        throw std::invalid_argument("latitudeSegments must be at least 2");
    }
    if (longitudeSegments < 3) {
        throw std::invalid_argument("longitudeSegments must be at least 3");
    }

    std::vector<Vertex> vertices;
    vertices.reserve(
        (latitudeSegments + 1) *
        (longitudeSegments + 1)
    );    
    for (std::uint32_t i = 0 ; i <= latitudeSegments ; i++) {
        for (std::uint32_t j = 0 ; j <= longitudeSegments ; j++) {
            float v = static_cast<float>(i)/static_cast<float>(latitudeSegments);
            float u = static_cast<float>(j)/static_cast<float>(longitudeSegments);
            float theta = v * std::numbers::pi_v<float>;
            float phi = u * 2.0f * std::numbers::pi_v<float>;
            const float x = std::sin(theta) * std::cos(phi);
            const float y = std::cos(theta);
            const float z = std::sin(theta) * std::sin(phi);
            vertices.push_back({
                Eigen::Vector3f(radius*x, radius*y, radius*z), 
                Eigen::Vector3f(x, y, z)
            });
        }
    }
    std::vector<std::uint32_t> indices;
    indices.reserve(
        static_cast<std::size_t>(latitudeSegments) *
        static_cast<std::size_t>(longitudeSegments) *
        6
    );
    for (std::uint32_t i = 0 ; i < latitudeSegments ; i++) {
        for (std::uint32_t j = 0 ; j < longitudeSegments ; j++) {
            const std::uint32_t stride = longitudeSegments + 1;
            const std::uint32_t a = i * stride + j;
            const std::uint32_t b = (i + 1) * stride + j;
            const std::uint32_t c = (i + 1) * stride + (j + 1);
            const std::uint32_t d = i * stride + (j + 1);

            indices.push_back(a);
            indices.push_back(c);
            indices.push_back(b);

            indices.push_back(a);
            indices.push_back(d);
            indices.push_back(c);            
        }
    }

    return Mesh(std::move(vertices), std::move(indices));   
}


Mesh createPlaneMesh(float halfExtent)
{
    if (halfExtent <= 0.0f) {
        throw std::invalid_argument("halfExtent must be positive");
    }
    const float h = halfExtent;

    std::vector<Vertex> vertices{
        {{-h, 0.0f, -h}, {0.0f, 1.0f, 0.0f}},
        {{-h, 0.0f,  h}, {0.0f, 1.0f, 0.0f}},
        {{ h, 0.0f,  h}, {0.0f, 1.0f, 0.0f}},
        {{ h, 0.0f, -h}, {0.0f, 1.0f, 0.0f}}
    };

    std::vector<std::uint32_t> indices{
        0, 1, 2,
        2, 3, 0
    };

    return Mesh(std::move(vertices), std::move(indices));
}
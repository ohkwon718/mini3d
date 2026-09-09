#pragma once

#include <vector>
#include <Eigen/Dense>
#include <cstdint>

struct Vertex
{
    Eigen::Vector3f position;
    Eigen::Vector3f normal;
    Eigen::Vector2f texCoord{0.0f, 0.0f};
};

class Mesh 
{
public:
    Mesh(std::vector<Vertex> vertices,
        std::vector<std::uint32_t> indices);

    const std::vector<Vertex>& vertices() const;
    const std::vector<std::uint32_t>& indices() const;

private:
    std::vector<Vertex> vertices_;
    std::vector<std::uint32_t> indices_;
};
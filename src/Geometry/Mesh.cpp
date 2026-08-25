#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices,
    std::vector<std::uint32_t> indices)
    : vertices_(std::move(vertices)),
      indices_(std::move(indices))
{
}

const std::vector<Vertex>& Mesh::vertices() const
{
    return vertices_;
}

const std::vector<std::uint32_t>& Mesh::indices() const
{
    return indices_;
}
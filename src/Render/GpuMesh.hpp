#pragma once

#include <cstddef>
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Geometry/Mesh.hpp"

class GpuMesh
{
public:
    explicit GpuMesh(const Mesh& mesh);

    void draw() const;

private:
    VertexArray vao_;
    VertexBuffer vbo_;
    IndexBuffer ebo_;
    std::size_t indexCount_{0};
};
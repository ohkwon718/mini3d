#include "GpuMesh.hpp"

#include <cstddef>
#include <glad/gl.h>


namespace
{
struct GpuVertex
{
    float px, py, pz;
    float nx, ny, nz;
    GpuVertex(const Vertex& vertex)
    : px(vertex.position.x()),
        py(vertex.position.y()),
        pz(vertex.position.z()),
        nx(vertex.normal.x()),
        ny(vertex.normal.y()),
        nz(vertex.normal.z())
    {}    
};
}

GpuMesh::GpuMesh(const Mesh& mesh)
    : indexCount_(mesh.indices().size())
{
    std::vector<GpuVertex> gpuVertices;
    gpuVertices.reserve(mesh.vertices().size());
    
    for (const auto& vertex : mesh.vertices()) {    
        gpuVertices.emplace_back(vertex);
    }


    vao_.bind();

    vbo_.upload(
        gpuVertices.data(),
        gpuVertices.size() * sizeof(GpuVertex)
    );

    ebo_.upload(
        mesh.indices().data(), 
        mesh.indices().size()
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLsizei>(sizeof(GpuVertex)),
        nullptr
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLsizei>(sizeof(GpuVertex)),
        reinterpret_cast<void*>(offsetof(GpuVertex, nx))
    );
    glEnableVertexAttribArray(1);

    VertexArray::unbind();
}


void GpuMesh::draw() const
{
    vao_.bind();
    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(indexCount_),
        GL_UNSIGNED_INT,
        nullptr
    );
}



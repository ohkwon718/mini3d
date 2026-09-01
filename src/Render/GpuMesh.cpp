#include "GpuMesh.hpp"

#include <glad/gl.h>

GpuMesh::GpuMesh(const Mesh& mesh)
    : indexCount_(mesh.indices().size())
{
    vao_.bind();

    vbo_.upload(
        mesh.vertices().data(),
        mesh.vertices().size() * sizeof(Vertex)
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
        sizeof(Vertex),
        nullptr
    );

    glEnableVertexAttribArray(0);

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



#include "GpuMesh.hpp"

#include <glad/gl.h>

GpuMesh::GpuMesh(const Mesh& mesh)
    : vertexCount_(mesh.vertices().size())
{
    vao_.bind();

    vbo_.upload(
        mesh.vertices().data(),
        mesh.vertices().size() * sizeof(Vertex)
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
    glDrawArrays(
        GL_TRIANGLES,
        0,
        static_cast<GLsizei>(vertexCount_)
    );
}



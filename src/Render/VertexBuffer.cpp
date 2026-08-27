#include "Render/VertexBuffer.hpp"
#include <glad/gl.h>

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &id_);
}

VertexBuffer::~VertexBuffer()
{
    if (id_ != 0) {
        glDeleteBuffers(1, &id_);
    }
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : id_(other.id_)
{
    other.id_ = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    if (this == &other) {
        return *this;
    }

    if (id_ != 0) {
        glDeleteBuffers(1, &id_);
    }
    id_ = other.id_;
    other.id_ = 0;

    return *this;
}

unsigned int VertexBuffer::id() const
{
    return id_;
}

void VertexBuffer::upload(const void* data, std::size_t sizeBytes)
{
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(
        GL_ARRAY_BUFFER, 
        static_cast<GLsizeiptr>(sizeBytes), 
        data, 
        GL_STATIC_DRAW
    );
}
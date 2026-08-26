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
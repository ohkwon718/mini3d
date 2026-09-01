#include "IndexBuffer.hpp"


IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &id_);
}

IndexBuffer::~IndexBuffer()
{
    if (id_ != 0) {
        glDeleteBuffers(1, &id_);
    }
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : id_(other.id_)
{
    other.id_ = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
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


void IndexBuffer::upload(const std::uint32_t* data, std::size_t count)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(count * sizeof(std::uint32_t)),
        data,
        GL_STATIC_DRAW
    );
}

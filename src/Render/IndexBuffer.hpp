#pragma once

#include <cstddef>
#include <cstdint>
#include <glad/gl.h>

class IndexBuffer
{
public:
    IndexBuffer();
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    IndexBuffer(IndexBuffer&& other) noexcept;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;

    void upload(const std::uint32_t* data, std::size_t count);

private:
    GLuint id_{0};
};
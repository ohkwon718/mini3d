#pragma once

#include <cstddef>

class VertexBuffer 
{
public:
    VertexBuffer();
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    unsigned int id() const;

    void upload(const void* data, std::size_t sizeBytes);

private:
    unsigned int id_{0};    
};
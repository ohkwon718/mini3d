#pragma once

#include <filesystem>
#include "core/Image.hpp"

class Texture2D
{
public:
    Texture2D();
    Texture2D(
        int width,
        int height,
        const unsigned char* pixels
    );

    Texture2D(const Image& image);

    ~Texture2D();

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

    void bind() const;
    
private:
    unsigned int id_{0};
};
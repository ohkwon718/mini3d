#pragma once

#include <filesystem>

class Texture2D
{
public:
    Texture2D(
        int width,
        int height,
        const unsigned char* pixels
    );

    ~Texture2D();

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

    void bind() const;

    static Texture2D fromFile(
        const std::filesystem::path& path
    );

private:
    unsigned int id_{0};
};
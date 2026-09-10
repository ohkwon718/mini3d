#pragma once

#include <vector>

class Image
{
public:
    Image(
        int width,
        int height,
        std::vector<unsigned char> pixels
    );

    int width() const;
    int height() const;

    const std::vector<unsigned char>& pixels() const;

private:
    int width_;
    int height_;
    std::vector<unsigned char> pixels_;
};
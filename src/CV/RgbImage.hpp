#pragma once

#include <cstdint>
#include <vector>

struct RgbImage
{
    int width;
    int height;
    std::vector<std::uint8_t> data;
};
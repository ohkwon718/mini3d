#pragma once

#include "ImageOrigin.hpp"

#include <cstddef>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>
#include <cstdint>

class RgbImage
{
public:
    RgbImage(
        int width,
        int height,
        ImageOrigin origin,
        std::vector<std::uint8_t> data);

    int width() const noexcept;
    int height() const noexcept;
    ImageOrigin origin() const noexcept;
    std::span<const std::uint8_t> row(std::size_t y) const noexcept;

    private:
    int width_;
    int height_;
    ImageOrigin origin_;

    std::vector<std::uint8_t> data_;

    std::ptrdiff_t firstRowOffset_{0};
    std::ptrdiff_t rowStride_{0};
    std::ptrdiff_t rowSize_{0};
};
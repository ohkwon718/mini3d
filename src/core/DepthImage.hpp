#pragma once

#include "ImageOrigin.hpp"

#include <cstddef>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>


class DepthImage
{
public:
    DepthImage(
        int width,
        int height,
        ImageOrigin origin,
        std::vector<float> data
    );

    int width() const noexcept;
    int height() const noexcept;
    ImageOrigin origin() const noexcept;

    std::span<float> row(std::size_t y) noexcept;
    std::span<const float> row(std::size_t y) const noexcept;


private:
    int width_;
    int height_;
    ImageOrigin origin_;

    std::vector<float> data_;
};


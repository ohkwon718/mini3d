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
        std::vector<float> data)
        : width_(width),
          height_(height),
          origin_(origin),
          data_(std::move(data))
    {
        if (width_ <= 0 || height_ <= 0) {
            throw std::invalid_argument(
                "DepthImage dimensions must be positive"
            );
        }

        const std::size_t widthSize = static_cast<std::size_t>(width_);
        const std::size_t heightSize = static_cast<std::size_t>(height_);

        if (data_.size() != widthSize * heightSize) {
            throw std::invalid_argument(
                "DepthImage data size does not match dimensions"
            );
        }

        rowSize_ = static_cast<std::ptrdiff_t>(widthSize);

        if (origin_ == ImageOrigin::TopLeft) {
            firstRowOffset_ = 0;
            rowStride_ = rowSize_;
        }
        else {
            firstRowOffset_ = static_cast<std::ptrdiff_t>((heightSize - 1) * widthSize);
            rowStride_ = -rowSize_;
        }
    }

    int width() const noexcept { return width_; }
    int height() const noexcept { return height_; }
    ImageOrigin origin() const noexcept { return origin_; }

    std::span<const float> row(std::size_t y) const noexcept
    {
        const std::ptrdiff_t offset = firstRowOffset_ + static_cast<std::ptrdiff_t>(y) * rowStride_;

        return {
            data_.data() + offset,
            static_cast<std::size_t>(width_)
        };
    }

private:
    int width_;
    int height_;
    ImageOrigin origin_;

    std::vector<float> data_;

    std::ptrdiff_t firstRowOffset_{0};
    std::ptrdiff_t rowStride_{0};
    std::ptrdiff_t rowSize_{0};
};
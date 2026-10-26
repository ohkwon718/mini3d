#include "DepthImage.hpp"
#include <fstream>


DepthImage::DepthImage(
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

}


int DepthImage::width() const noexcept 
{ 
    return width_; 
}


int DepthImage::height() const noexcept 
{ 
    return height_; 
}


ImageOrigin DepthImage::origin() const noexcept 
{ 
    return origin_; 
}


std::span<float> DepthImage::row(std::size_t y) noexcept
{
    const std::size_t rowSize = static_cast<std::size_t>(width_);

    return {data_.data() + y * rowSize, rowSize};
}

std::span<const float> DepthImage::row(std::size_t y) const noexcept
{
    const std::size_t rowSize = static_cast<std::size_t>(width_);

    return {data_.data() + y * rowSize, rowSize};
}



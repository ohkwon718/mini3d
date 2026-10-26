#include "RgbImage.hpp"


RgbImage::RgbImage(
    int width,
    int height,
    ImageOrigin origin,
    std::vector<std::uint8_t> data)
    : width_(width),
        height_(height),
        origin_(origin),
        data_(std::move(data))
{
    if (width_ <= 0 || height_ <= 0) {
        throw std::invalid_argument(
            "RgbImage dimensions must be positive"
        );
    }

    const std::size_t widthSize = static_cast<std::size_t>(width_);
    const std::size_t heightSize = static_cast<std::size_t>(height_);

    if (data_.size() != widthSize * heightSize * 3) {
        throw std::invalid_argument(
            "RgbImage data size does not match dimensions"
        );
    }
}

int RgbImage::width() const noexcept 
{ 
    return width_; 
}

int RgbImage::height() const noexcept 
{ 
    return height_; 
}

ImageOrigin RgbImage::origin() const noexcept 
{ 
    return origin_; 
}

std::span<const std::uint8_t> RgbImage::row(std::size_t y) const noexcept
{
    const std::size_t rowSize = static_cast<std::size_t>(width_) * 3;

    return {data_.data() + y * rowSize, rowSize};
}
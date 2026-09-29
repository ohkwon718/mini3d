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


std::span<const float> DepthImage::row(std::size_t y) const noexcept
{
    const std::ptrdiff_t offset = firstRowOffset_ + static_cast<std::ptrdiff_t>(y) * rowStride_;

    return {
        data_.data() + offset,
        static_cast<std::size_t>(width_)
    };
}



void saveDepthPreview(
    const DepthImage& depth,
    const Camera& camera,
    const std::string& path)
{
    constexpr float maxVisualDepth = 10.0f;

    const std::size_t width =
        static_cast<std::size_t>(depth.width());

    const std::size_t height =
        static_cast<std::size_t>(depth.height());

    std::vector<std::uint8_t> pixels(width * height);

    for (std::size_t y = 0; y < height; ++y) {

        const auto depthRow = depth.row(y);

        for (std::size_t x = 0; x < width; ++x) {

            const float raw = depthRow[x];
            const std::size_t index = y * width + x;

            if (raw >= 1.0f) {
                pixels[index] = 0;
                continue;
            }

            const float metricDepth =
                camera.depthToMetric(raw);

            const float normalized =
                1.0f - std::clamp(
                    metricDepth / maxVisualDepth,
                    0.0f,
                    1.0f
                );

            pixels[index] =
                static_cast<std::uint8_t>(
                    normalized * 255.0f
                );
        }
    }


    std::ofstream file(path, std::ios::binary);

    file << "P5\n"
         << width << ' ' << height << "\n255\n";

    file.write(
        reinterpret_cast<const char*>(pixels.data()),
        static_cast<std::streamsize>(pixels.size())
    );
}
#include "OpenCvInterop.hpp"

cv::Mat toCvMatCopy(const RgbImage& image)
{    
    cv::Mat mat(image.height(), image.width(), CV_8UC3);

    for (std::size_t y = 0; y < static_cast<std::size_t>(image.height()); ++y) {
        const auto src = image.row(y);
        std::copy(src.begin(), src.end(), mat.ptr<std::uint8_t>(static_cast<int>(y)));
    }

    return mat;
}
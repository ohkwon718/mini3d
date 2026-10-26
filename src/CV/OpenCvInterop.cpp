#include "OpenCvInterop.hpp"

cv::Mat toCvMatCopy(const RgbImage& image)
{    
    cv::Mat mat(image.height(), image.width(), CV_8UC3);
    
    if (image.origin() == ImageOrigin::TopLeft){
        for (std::size_t y = 0; y < static_cast<std::size_t>(image.height()); ++y) {        
            const auto src = image.row(y);
            std::copy(src.begin(), src.end(), mat.ptr<std::uint8_t>(static_cast<int>(y)));
        }
    }
    else if (image.origin() == ImageOrigin::BottomLeft){
        for (std::size_t y = 0; y < static_cast<std::size_t>(image.height()); ++y) {        
            const auto src = image.row(image.height() - 1 - y);
            std::copy(src.begin(), src.end(), mat.ptr<std::uint8_t>(static_cast<int>(y)));
        }
    }

    return mat;
}
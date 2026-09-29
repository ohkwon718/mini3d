#pragma once

#include <opencv2/core.hpp>
#include "core/RgbImage.hpp"

cv::Mat toCvMatCopy(const RgbImage& image);
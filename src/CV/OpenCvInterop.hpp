#pragma once

#include <opencv2/core.hpp>
#include "RgbImage.hpp"

cv::Mat toCvMatCopy(const RgbImage& image);
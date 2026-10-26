#pragma once

#include "Camera/CameraIntrinsics.hpp"
#include "Transform/Pose.hpp"
#include "core/RgbImage.hpp"
#include "core/DepthImage.hpp"

struct CameraFrame {
    std::size_t frameIndex;
    double timestamp;

    Pose pose;
    CameraIntrinsics intrinsics;

    RgbImage rgb;
    DepthImage rawDepth; // Raw OpenGL depth buffer values [0, 1]
};
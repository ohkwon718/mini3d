#pragma once

#include "CameraFrame.hpp"
#include "Render/RenderTarget.hpp"
#include "Camera/Camera.hpp"

#include <cstddef>


CameraFrame captureCameraFrame(
    const RenderTarget& target,
    const Camera& camera,
    std::size_t frameIndex,
    double timestamp
);

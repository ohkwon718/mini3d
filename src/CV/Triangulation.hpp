#pragma once

#include <Eigen/Dense>
#include <Camera/Camera.hpp>

Eigen::Vector3f triangulateLinearSvd(
    const Camera& cameraA,
    const Eigen::Vector2f& pixelA,
    const Camera& cameraB,
    const Eigen::Vector2f& pixelB);

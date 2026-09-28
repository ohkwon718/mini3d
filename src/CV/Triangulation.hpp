#pragma once

#include <span>
#include <Eigen/Dense>
#include <Camera/Camera.hpp>


struct CameraObservation
{
    const Camera* camera;
    Eigen::Vector2f pixel;
};


Eigen::Vector3f triangulateLinearSvd(
    std::span<const CameraObservation> observations    
);


Eigen::Vector3f triangulateClosestRays(
    const Camera& cameraA,
    const Eigen::Vector2f& pixelA,
    const Camera& cameraB,
    const Eigen::Vector2f& pixelB
);
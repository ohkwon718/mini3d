#include "Camera/Camera.hpp"
#include "CV/Triangulation.hpp"
#include <Eigen/Dense>
#include <cassert>
#include <numbers>
#include <stdexcept>
#include <iostream>

int main() {
    CameraIntrinsics intrinsics{
        800,
        600,
        724.264f,
        724.264f,
        400.0f,
        300.0f
    };

    Camera cameraA(
        intrinsics,
        0.1f,
        100.0f
    );
    cameraA.setPosition({-0.5f, 0.0f, 3.0f});
    cameraA.setRotation(Eigen::Quaternionf(Eigen::AngleAxisf(-0.1f, Eigen::Vector3f::UnitY())));

    Camera cameraB(
        intrinsics,
        0.1f,
        100.0f
    );
    cameraB.setPosition({ 0.5f, 0.0f, 3.0f});
    cameraB.setRotation(Eigen::Quaternionf(Eigen::AngleAxisf(0.1f, Eigen::Vector3f::UnitY())));

    const Eigen::Vector3f worldPoint(1.5f, 2.3f, -4.0f);
    Eigen::Vector2f pixelA = cameraA.project(worldPoint);
    Eigen::Vector2f pixelB = cameraB.project(worldPoint);
    
    const std::array<CameraObservation, 2> observations{{
        {&cameraA, pixelA},
        {&cameraB, pixelB}
    }};
    Eigen::Vector3f reconstructed = triangulateLinearSvd(observations);    
    assert(reconstructed.isApprox(worldPoint, 1e-4f));

    reconstructed = triangulateClosestRays(cameraA, pixelA, cameraB, pixelB);
    assert(reconstructed.isApprox(worldPoint, 1e-4f));

    return 0;
}


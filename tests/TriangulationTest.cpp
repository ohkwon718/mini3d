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

    Camera cameraB(
        intrinsics,
        0.1f,
        100.0f
    );
    cameraB.setPosition({ 0.5f, 0.0f, 3.0f});

    const Eigen::Vector3f worldPoint(1.5f, 2.3f, -4.0f);
    Eigen::Vector2f pixelA = cameraA.project(worldPoint);
    Eigen::Vector2f pixelB = cameraB.project(worldPoint);
    
    Eigen::Vector3f reconstructed = triangulateLinearSvd(cameraA, pixelA, cameraB, pixelB);    
    assert(reconstructed.isApprox(worldPoint, 1e-4f));
    
    reconstructed = triangulateClosestRays(cameraA, pixelA, cameraB, pixelB);
    assert(reconstructed.isApprox(worldPoint, 1e-4f));


    cameraA.setRotation(Eigen::Quaternionf(Eigen::AngleAxisf(-0.1f, Eigen::Vector3f::UnitY())));
    cameraB.setRotation(Eigen::Quaternionf(Eigen::AngleAxisf(0.1f, Eigen::Vector3f::UnitY())));
    pixelA = cameraA.project(worldPoint);
    pixelB = cameraB.project(worldPoint);    

    reconstructed = triangulateLinearSvd(cameraA, pixelA, cameraB, pixelB);    
    assert(reconstructed.isApprox(worldPoint, 1e-4f));

    reconstructed = triangulateClosestRays(cameraA, pixelA, cameraB, pixelB);
    assert(reconstructed.isApprox(worldPoint, 1e-4f));


    return 0;
}


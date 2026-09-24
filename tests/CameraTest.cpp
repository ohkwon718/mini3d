#include "Camera/Camera.hpp"
#include <Eigen/Dense>
#include <cassert>
#include <numbers>
#include <stdexcept>
#include <iostream>

int main() {
    const CameraIntrinsics intrinsics{
        1280, 720, 869.116f, 869.116f, 640.0f, 360.0f
    };
    Camera camera(intrinsics, 0.1f, 100.0f);
    assert(camera.viewMatrix().isApprox(Eigen::Matrix4f::Identity()));
    camera.setPosition(Eigen::Vector3f(1.0f, 2.0f, 3.0f));
    
    Eigen::Matrix4f expected;
    expected << 1.0f, 0.0f, 0.0f, -1.0f,
                0.0f, 1.0f, 0.0f, -2.0f,
                0.0f, 0.0f, 1.0f, -3.0f,
                0.0f, 0.0f, 0.0f, 1.0f;    
    assert(camera.viewMatrix().isApprox(expected));
    

    CameraIntrinsics badIntrinsics = intrinsics;
    badIntrinsics.fx = 0.0f;
    bool threw = false;
    try {
        Camera badCamera(badIntrinsics, 10.0f, 1.0f);
    }
    catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    assert(std::abs(camera.depthToMetric(0.0f) - 0.1f) < 1e-5f);
    assert(std::abs(camera.depthToMetric(1.0f) - 100.0f) < 1e-3f);
        
    Eigen::Vector2f pixel = camera.project(Eigen::Vector3f(1.0f, 2.0f, 1.0f));
    assert(pixel.isApprox(
        Eigen::Vector2f(intrinsics.cx, intrinsics.cy)
    ));


    const Eigen::Vector3f worldPoint(1.5f, 2.3f, -4.0f);
    pixel = camera.project(worldPoint);

    const auto veiwMatrix = camera.viewMatrix();
    const Eigen::Vector4f homoWorldPoint = Eigen::Vector4f(worldPoint.x(), worldPoint.y(), worldPoint.z(), 1.0f);
    const Eigen::Vector4f homoCamCoord = veiwMatrix * homoWorldPoint;   
    const float depth = -homoCamCoord.z();
    const Eigen::Vector3f reconstructed = camera.unproject(pixel, depth);
    assert(reconstructed.isApprox(worldPoint, 1e-5f));    
    
    Eigen::Vector4f homoNDC = camera.projectionMatrix() * homoCamCoord;
    homoNDC /= homoNDC.w();
    float u2 = (homoNDC.x() + 1.0f) * static_cast<float>(camera.intrinsics().width) * 0.5f - 0.5f;
    float v2 = (1.0f - homoNDC.y()) * static_cast<float>(camera.intrinsics().height) * 0.5f - 0.5f;    
    assert(pixel.isApprox(Eigen::Vector2f(u2, v2)));
    
    return 0;
}


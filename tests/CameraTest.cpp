#include "Camera/Camera.hpp"
#include <cassert>
#include <numbers>
#include <stdexcept>

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


    bool threw = false;
    try {
        Camera badCamera(intrinsics, 10.0f, 1.0f);
    }
    catch (const std::invalid_argument&) {
        threw = true;
    }

    assert(threw);

    return 0;
}


#include <cassert>
#include "Camera/Camera.hpp"
#include <numbers>

int main() {
    Camera camera(std::numbers::pi_v<float> / 4.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    assert(camera.viewMatrix().isApprox(Eigen::Matrix4f::Identity()));
    camera.setPosition(Eigen::Vector3f(1.0f, 2.0f, 3.0f));
    
    Eigen::Matrix4f expected;
    expected << 1.0f, 0.0f, 0.0f, -1.0f,
                0.0f, 1.0f, 0.0f, -2.0f,
                0.0f, 0.0f, 1.0f, -3.0f,
                0.0f, 0.0f, 0.0f, 1.0f;
    
    assert(camera.viewMatrix().isApprox(expected));

    return 0;
}
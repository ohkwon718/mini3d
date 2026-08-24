#include <cassert>
#include <numbers>
#include "Transform/Transform.hpp"

int main() {
    Transform t;
    
    assert(t.translation().isApprox(Eigen::Vector3f(0.0f, 0.0f, 0.0f)));
    assert(t.rotation().isApprox(Eigen::Quaternionf(1.0f, 0.0f, 0.0f, 0.0f)));
    assert(t.scale().isApprox(Eigen::Vector3f(1.0f, 1.0f, 1.0f)));
    assert(t.matrix().isApprox(Eigen::Matrix4f::Identity()));

    t.setTranslation(Eigen::Vector3f(10.0f, 0.0f, 0.0f));
    t.setRotation(Eigen::Quaternionf(1.0f, 0.0f, 0.0f, 0.0f)); // identity quaternion
    t.setScale(Eigen::Vector3f(2.0f, 3.0f, 4.0f));    
    Eigen::Matrix4f expected1;
    expected1 << 2.0f, 0.0f, 0.0f, 10.0f,
                 0.0f, 3.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 4.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f;
    assert(t.matrix().isApprox(expected1));

    Eigen::Vector4f p(1.0f, 1.0f, 1.0f, 1.0f);
    Eigen::Vector4f result = t.matrix() * p;
    assert(result.isApprox(Eigen::Vector4f(12.0f, 3.0f, 4.0f, 1.0f)));
    

    Eigen::Vector4f point(1.0f, 0.0f, 0.0f, 1.0f);
    t.setTranslation(Eigen::Vector3f(0.0f, 0.0f, 0.0f));
    // rotate 90 deg around Z-axis
    Eigen::AngleAxisf angle_axis(
        std::numbers::pi_v<float> / 2.0f,
        Eigen::Vector3f::UnitZ()
    );
    t.setRotation(Eigen::Quaternionf(angle_axis));
    t.setScale(Eigen::Vector3f(1.0f, 1.0f, 1.0f));

    Eigen::Vector4f expected2(0.0f, 1.0f, 0.0f, 1.0f);
    result = t.matrix() * point;
    assert(result.isApprox(expected2));


    return 0;
}
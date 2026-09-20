#include "Camera.hpp"
#include <stdexcept>
#include <cmath>
#include <numbers>


Camera::Camera(
    const CameraIntrinsics& intrinsics,
    float nearPlane,
    float farPlane)
    : intrinsics_(intrinsics),
      nearPlane_(nearPlane),
      farPlane_(farPlane)
{
    if ( nearPlane <= 0.0f ) {
        throw std::invalid_argument( "received invalid near plane" );
    }
    if ( farPlane <= nearPlane ) {
        throw std::invalid_argument( "received invalid far plane" );
    }
}

Eigen::Vector3f Camera::position() const
{
    return position_;
}

Eigen::Quaternionf Camera::rotation() const
{
    return orientation_;
}

void Camera::setPosition(const Eigen::Vector3f& position)
{
    position_ = position;
}

void Camera::setRotation(const Eigen::Quaternionf& rotation)
{
    const float norm = rotation.norm();
    if (!std::isfinite(norm) || norm < 1e-6f) {
        throw std::invalid_argument("Quaternion must have a non-zero finite norm");
    }
    orientation_ = rotation.normalized();
}

Eigen::Matrix4f Camera::viewMatrix() const
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
    Eigen::Matrix3f rotationMatrix = orientation_.toRotationMatrix();
    view.block<3, 3>(0, 0) = rotationMatrix.transpose();
    view.block<3, 1>(0, 3) = -rotationMatrix.transpose() * position_;
    return view;
}

Eigen::Matrix4f Camera::projectionMatrix() const
{
    const float width = static_cast<float>(intrinsics_.width);
    const float height = static_cast<float>(intrinsics_.height);

    const float n = nearPlane_;
    const float f = farPlane_;

    Eigen::Matrix4f proj = Eigen::Matrix4f::Zero();

    proj(0, 0) = 2.0f * intrinsics_.fx / width;
    proj(1, 1) = 2.0f * intrinsics_.fy / height;
    proj(0, 2) = 1.0f - 2.0f * intrinsics_.cx / width;
    proj(1, 2) = 2.0f * intrinsics_.cy / height - 1.0f;
    proj(2, 2) = (f + n) / (n - f);
    proj(2, 3) = 2.0f * f * n / (n - f);
    proj(3, 2) = -1.0f;

    return proj;
}

const CameraIntrinsics &Camera::intrinsics() const noexcept
{
    return intrinsics_;
}

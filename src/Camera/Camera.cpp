#include "Camera.hpp"
#include <stdexcept>
#include <cmath>
#include <numbers>

Camera::Camera(float verticalFov,
    float aspectRatio,
    float nearPlane,
    float farPlane)
    : position_(Eigen::Vector3f::Zero()),
      orientation_(Eigen::Quaternionf::Identity()),
      verticalFov_(verticalFov),
      aspectRatio_(aspectRatio),
      nearPlane_(nearPlane),
      farPlane_(farPlane)
{    
    if ( verticalFov <= 0.0f || verticalFov >= std::numbers::pi_v<float> ) {
        throw std::invalid_argument( "received invalid vertical field of view" );
    }
    if ( aspectRatio <= 0.0f ) {
        throw std::invalid_argument( "received invalid aspect ratio" );
    }
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

void Camera::setAspectRatio(float aspectRatio)
{
    if ( aspectRatio <= 0.0f ) {
        throw std::invalid_argument( "received invalid aspect ratio" );
    }
    aspectRatio_ = aspectRatio;
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
    float f = 1.0f / std::tan(verticalFov_ / 2.0f);
    Eigen::Matrix4f proj = Eigen::Matrix4f::Zero();
    proj(0, 0) = f / aspectRatio_;
    proj(1, 1) = f;
    proj(2, 2) = (farPlane_ + nearPlane_) / (nearPlane_ - farPlane_);
    proj(2, 3) = (2.0f * farPlane_ * nearPlane_) / (nearPlane_ - farPlane_);
    proj(3, 2) = -1.0f;
    return proj;
}
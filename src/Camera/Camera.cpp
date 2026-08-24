#include "Camera.hpp"

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
    orientation_ = rotation;
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
#include "Transform.hpp"
#include <cmath>
#include <stdexcept>

Transform::Transform()
    : translation_(Eigen::Vector3f::Zero()),
      rotation_(Eigen::Quaternionf::Identity()),
      scale_(Eigen::Vector3f::Ones())
{    
}

void Transform::setTranslation(const Eigen::Vector3f &translation)
{
    translation_ = translation;
}

void Transform::setRotation(const Eigen::Quaternionf &rotation)
{
    const float norm = rotation.norm();
    if (!std::isfinite(norm) || norm < 1e-6f) {
        throw std::invalid_argument("Quaternion must have a non-zero finite norm");
    }
    rotation_ = rotation.normalized();
}

void Transform::setScale(const Eigen::Vector3f &scale)
{
    scale_ = scale;
}

Eigen::Vector3f Transform::translation() const
{
    return translation_;
}

Eigen::Quaternionf Transform::rotation() const
{
    return rotation_;
}

Eigen::Vector3f Transform::scale() const
{
    return scale_;
}


Eigen::Matrix4f Transform::matrix() const
{    
    Eigen::Matrix4f transformMatrix = Eigen::Matrix4f::Identity();
    Eigen::Matrix3f rotationMatrix = rotation_.toRotationMatrix();
    transformMatrix.block<3, 3>(0, 0) = rotationMatrix * scale_.asDiagonal();
    transformMatrix.block<3, 1>(0, 3) = translation_;
    return transformMatrix;
}

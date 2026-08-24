#include "Transform.hpp"
#include <iostream>


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
    rotation_ = rotation;
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

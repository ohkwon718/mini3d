#pragma once

#include <Eigen/Dense>
#include <Camera/CameraIntrinsics.hpp>

class Camera 
{
public:
    Camera(
        const CameraIntrinsics& intrinsics,
        float nearPlane,
        float farPlane
    );

    Eigen::Vector3f position() const;
    Eigen::Quaternionf rotation() const;

    void setPosition(const Eigen::Vector3f&);
    void setRotation(const Eigen::Quaternionf&);

    Eigen::Matrix4f viewMatrix() const;
    Eigen::Matrix4f projectionMatrix() const;
    const CameraIntrinsics& intrinsics() const noexcept;

private:
    Eigen::Vector3f position_{Eigen::Vector3f::Zero()};
    Eigen::Quaternionf orientation_{Eigen::Quaternionf::Identity()};    

    CameraIntrinsics intrinsics_;
    float nearPlane_;
    float farPlane_;
};
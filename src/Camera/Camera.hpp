#pragma once

#include <Eigen/Dense>
#include <Camera/CameraIntrinsics.hpp>
#include <Transform/Pose.hpp>

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

    void setPosition(const Eigen::Vector3f& position);
    void setRotation(const Eigen::Quaternionf& rotation);
    void setPose(const Pose& pose);

    Eigen::Matrix4f viewMatrix() const;
    Eigen::Matrix4f projectionMatrix() const;
    Eigen::Matrix<float, 3, 4> cameraMatrix() const;
    const CameraIntrinsics& intrinsics() const noexcept;

    float depthToMetric(float depth) const;
    Eigen::Vector2f project(const Eigen::Vector3f& worldPoint) const;
    Eigen::Vector3f unproject(const Eigen::Vector2f& pixel, float depth) const;

private:
    Eigen::Vector3f position_{Eigen::Vector3f::Zero()};
    Eigen::Quaternionf orientation_{Eigen::Quaternionf::Identity()};    

    CameraIntrinsics intrinsics_;
    float nearPlane_;
    float farPlane_;
};
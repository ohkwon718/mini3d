#pragma once

#include <Eigen/Dense>

class Camera 
{
public:
    Camera(float verticalFovRadians,
        float aspectRatio,
        float nearPlane,
        float farPlane);

    Eigen::Vector3f position() const;
    Eigen::Quaternionf rotation() const;

    void setPosition(const Eigen::Vector3f&);
    void setRotation(const Eigen::Quaternionf&);

    void setAspectRatio(float aspectRatio);

    Eigen::Matrix4f viewMatrix() const;
    Eigen::Matrix4f projectionMatrix() const;

private:
    Eigen::Vector3f position_;
    Eigen::Quaternionf orientation_;
    float verticalFov_;
    float aspectRatio_;
    float nearPlane_;
    float farPlane_;

};
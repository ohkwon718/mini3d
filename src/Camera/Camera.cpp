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
    if (intrinsics.width <= 0 || intrinsics.height <= 0 ||
        intrinsics.fx <= 0.0f || intrinsics.fy <= 0.0f) {
        throw std::invalid_argument("Invalid camera intrinsics");
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

    const float cx = intrinsics_.cx + 0.5f;
    const float cy = intrinsics_.cy + 0.5f;

    proj(0, 0) = 2.0f * intrinsics_.fx / width;
    proj(1, 1) = 2.0f * intrinsics_.fy / height;
    proj(0, 2) = 1.0f - 2.0f * cx / width;
    proj(1, 2) = 2.0f * cy / height - 1.0f;
    proj(2, 2) = (f + n) / (n - f);
    proj(2, 3) = 2.0f * f * n / (n - f);
    proj(3, 2) = -1.0f;

    return proj;
}



Eigen::Matrix<float, 3, 4> Camera::cameraMatrix() const
{
    Eigen::Matrix<float, 3, 4> viewMatrix3x4;

    Eigen::Matrix3f rotationMatrix = orientation_.toRotationMatrix();
    viewMatrix3x4.block<3, 3>(0, 0) = rotationMatrix.transpose();
    viewMatrix3x4.block<3, 1>(0, 3) = -rotationMatrix.transpose() * position_;

    Eigen::Matrix3f glToCv = Eigen::Matrix3f::Identity();
    glToCv(1, 1) = -1.0f;
    glToCv(2, 2) = -1.0f;

    Eigen::Matrix3f K = Eigen::Matrix3f::Zero();
    K(0, 0) = intrinsics_.fx;
    K(1, 1) = intrinsics_.fy;
    K(0, 2) = intrinsics_.cx;
    K(1, 2) = intrinsics_.cy;
    K(2, 2) = 1.0f;

    return K * glToCv * viewMatrix3x4;
}


const CameraIntrinsics &Camera::intrinsics() const noexcept
{
    return intrinsics_;
}

float Camera::depthToMetric(float depth) const
{    
    const double d = static_cast<double>(depth);
    const double n = static_cast<double>(nearPlane_);
    const double f = static_cast<double>(farPlane_);
    const double zNdc = 2.0 * d - 1.0;

    const double z = (2.0f * n * f) / 
        (f + n - zNdc * (f - n));

    return static_cast<float>(z);
}

Eigen::Vector2f Camera::project(const Eigen::Vector3f &worldPoint) const
{
    const Eigen::Vector4f homoWorldPoint(
        worldPoint.x(),
        worldPoint.y(),
        worldPoint.z(),
        1.0f
    );

    const Eigen::Vector3f cameraPoint = (viewMatrix() * homoWorldPoint).head<3>();
    const float depth = -cameraPoint.z();
    return Eigen::Vector2f(
        intrinsics_.cx + intrinsics_.fx * cameraPoint.x() / depth,  
        intrinsics_.cy - intrinsics_.fy * cameraPoint.y() / depth
    );    
}

Eigen::Vector3f Camera::unproject(const Eigen::Vector2f& pixel, float depth) const
{
    const Eigen::Vector3f cameraPoint(
        depth * (pixel.x() - intrinsics_.cx) / intrinsics_.fx,
        -depth * (pixel.y() - intrinsics_.cy) / intrinsics_.fy,
        -depth
    );
    return position_ + orientation_ * cameraPoint;
}

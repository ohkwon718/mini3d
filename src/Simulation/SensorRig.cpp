#include "SensorRig.hpp"
#include <cmath>
#include <stdexcept>

SensorRig::SensorRig(
    Eigen::Vector3f position,
    Eigen::Quaternionf orientation)
: position_{position}
{
    setRotation(orientation);
}


Eigen::Vector3f SensorRig::position() const
{
    return position_;
}

Eigen::Quaternionf SensorRig::rotation() const
{
    return orientation_;
}

void SensorRig::setPosition(const Eigen::Vector3f& position)
{
    position_ = position;
}

void SensorRig::setRotation(const Eigen::Quaternionf& rotation)
{
    const float norm = rotation.norm();
    if (!std::isfinite(norm) || norm < 1e-6f) {
        throw std::invalid_argument("Quaternion must have a non-zero finite norm");
    }
    orientation_ = rotation.normalized();
}

void SensorRig::setPose(const Pose& pose)
{
    setPosition(pose.position);
    setRotation(pose.orientation);
}

void SensorRig::addCamera(const std::string name, MountedCamera camera)
{
    auto [it, inserted] = cameras_.emplace(std::move(name), std::move(camera));
    if (!inserted) {
        throw std::runtime_error("Duplicate camera name: " + it->first);
    }
}

Camera SensorRig::camera(const std::string& name) const
{
    auto it = cameras_.find(name);    
    if (it == cameras_.end()) {
        throw std::runtime_error("No camera name: " + name);        
    }
    Camera camera(it->second.camera);
    camera.setPosition(position_ + orientation_ * it->second.localPosition);
    camera.setRotation(orientation_ * it->second.localOrientation);

    return camera;
}

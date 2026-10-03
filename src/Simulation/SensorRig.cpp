#include "SensorRig.hpp"

#include <stdexcept>

SensorRig::SensorRig(
    Eigen::Vector3f position,
    Eigen::Quaternionf orientation)
: position_{position}, orientation_{orientation}
{

}

void SensorRig::addCamera(const std::string& name, MountedCamera camera)
{
    auto [it, inserted] = cameras_.emplace(std::move(name), std::move(camera));
    if (!inserted) {
        throw std::runtime_error("Duplicate camera name: " + it->first);
    }
}

const Camera& SensorRig::camera(const std::string& name) const
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

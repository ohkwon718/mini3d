#pragma once

#include <string>
#include <unordered_map>
#include "Camera/Camera.hpp"
#include "Transform/Transform.hpp"

struct MountedCamera
{
    Camera camera;
    Eigen::Vector3f localPosition{Eigen::Vector3f::Zero()};
    Eigen::Quaternionf localOrientation{Eigen::Quaternionf::Identity()};
};


class SensorRig
{
public:
    SensorRig(
        Eigen::Vector3f position,
        Eigen::Quaternionf orientation
    );

    Eigen::Vector3f position() const;
    Eigen::Quaternionf rotation() const;

    void setPosition(const Eigen::Vector3f&);
    void setRotation(const Eigen::Quaternionf&);

    void addCamera(const std::string& name, MountedCamera camera);
    Camera camera(const std::string& name) const;

private:    
    std::unordered_map<std::string, MountedCamera> cameras_;

    Eigen::Vector3f position_{Eigen::Vector3f::Zero()};
    Eigen::Quaternionf orientation_{Eigen::Quaternionf::Identity()};    

};



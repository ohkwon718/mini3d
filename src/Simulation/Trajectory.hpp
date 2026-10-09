#pragma once

#include <string>
#include <vector>
#include "Simulation/SensorRig.hpp"
#include "Camera/Camera.hpp"

struct KeyFrame
{
    double time;
    Eigen::Vector3f position;
    Eigen::Quaternionf orientation;
};


class Trajectory
{
public:    
    Trajectory(std::string rigName);    
    void addKeyframe(KeyFrame keyframe);
    const std::string& rigName() const;
    const Pose sample(double time) const;
    double duration() const;
    
private:
    std::string rigName_;
    std::vector<KeyFrame> keyframes_;
};



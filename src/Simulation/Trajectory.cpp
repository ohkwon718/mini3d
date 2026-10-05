#include "Trajectory.hpp"

#include <stdexcept>
#include <iostream>
Trajectory::Trajectory(std::string rigName)
: rigName_(std::move(rigName))
{

}

void Trajectory::addKeyframe(KeyFrame keyframe)
{
    keyframes_.push_back(keyframe);    
}

const std::string& Trajectory::rigName() const
{
    return rigName_;
}

const Pose Trajectory::sample(float time) const
{
    std::size_t index = 0;
    while (keyframes_[index].time > time && index < keyframes_.size()){
        index++;
    }

    if (index >= keyframes_.size()-1) {
        throw std::invalid_argument("The input time is not in the keyframe range");
    }
    
    float alpha = (time - keyframes_[index].time)/(keyframes_[index+1].time - keyframes_[index].time);
    return Pose{
        (1-alpha)*keyframes_[index].position + alpha * keyframes_[index+1].position,
        keyframes_[index].orientation.slerp(alpha, keyframes_[index+1].orientation)
    };
   
}
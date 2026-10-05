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
    
    if (time <= keyframes_.front().time) {
        return Pose{
            keyframes_.front().position,
            keyframes_.front().orientation,
        };
    }
    if (time >= keyframes_.back().time) {
        return Pose{
            keyframes_.back().position,
            keyframes_.back().orientation,
        };
    }

    while (keyframes_[index].time > time && index < keyframes_.size()-1){
        index++;
    }
    
    float alpha = (time - keyframes_[index].time)/(keyframes_[index+1].time - keyframes_[index].time);
    return Pose{
        (1-alpha)*keyframes_[index].position + alpha * keyframes_[index+1].position,
        keyframes_[index].orientation.slerp(alpha, keyframes_[index+1].orientation)
    };
   
}
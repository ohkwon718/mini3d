#include "Trajectory.hpp"

#include <stdexcept>
#include <iostream>


Trajectory::Trajectory(std::string rigName)
: rigName_(std::move(rigName))
{

}


void Trajectory::addKeyframe(KeyFrame keyframe)
{
    if (keyframes_.size() > 0 && keyframes_.back().time >= keyframe.time)
    {
        throw std::invalid_argument("The new keyframe must have later time than previous keyframes");
    }
    keyframes_.push_back(keyframe);

}


const std::string& Trajectory::rigName() const
{
    return rigName_;
}


const Pose Trajectory::sample(double time) const
{    
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

    std::size_t index = 1;
    while (keyframes_[index].time < time && index < keyframes_.size()){
        index++;
    }
    index--;    
    
    float alpha = static_cast<float>((time - keyframes_[index].time)/(keyframes_[index+1].time - keyframes_[index].time));
    return Pose{
        (1-alpha)*keyframes_[index].position + alpha * keyframes_[index+1].position,
        keyframes_[index].orientation.slerp(alpha, keyframes_[index+1].orientation)
    };   
}


double Trajectory::duration() const
{
    return keyframes_.back().time;
}


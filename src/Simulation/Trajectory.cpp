#include "Trajectory.hpp"


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

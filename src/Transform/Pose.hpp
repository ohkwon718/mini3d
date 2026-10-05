#pragma once

#include <Eigen/Dense>

struct Pose
{
    Eigen::Vector3f position;
    Eigen::Quaternionf orientation;
};
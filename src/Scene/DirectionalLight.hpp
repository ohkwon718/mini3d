#pragma once

#include <Eigen/Dense>

struct DirectionalLight
{
    Eigen::Vector3f direction;
    Eigen::Vector3f color;
    float intensity;
};
#pragma once

#include <Eigen/Dense>

struct Material
{
    Eigen::Vector3f baseColor{1.0f, 1.0f, 1.0f};

    Material() = default;

    Material(float r, float g, float b)
        : baseColor(r, g, b)
    {
    }
};
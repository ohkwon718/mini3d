#pragma once

#include <Eigen/Dense>

struct Material
{
    Eigen::Vector3f baseColor{1.0f, 1.0f, 1.0f};
    float shininess{32.0f};
    float specularStrength{0.5f};
    bool useTexture{false};

    Material() = default;

    Material(
        float r,
        float g,
        float b,
        float shininessValue = 32.0f,
        float specularStrengthValue = 0.5f,
        bool useTextureValue = false
    )
        : baseColor(r, g, b),
        shininess(shininessValue),
        specularStrength(specularStrengthValue),
        useTexture(useTextureValue)
    {
    }
};
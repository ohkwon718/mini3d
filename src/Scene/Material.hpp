#pragma once

#include <Eigen/Dense>
#include <utility>
#include "core/Image.hpp"

struct Material
{
    Eigen::Vector3f baseColor{1.0f, 1.0f, 1.0f};
    float shininess{32.0f};
    float specularStrength{0.5f};
    std::shared_ptr<const Image> image;

    Material() = default;

    Material(
        float r,
        float g,
        float b,
        float shininessValue = 32.0f,
        float specularStrengthValue = 0.5f,        
        std::shared_ptr<const Image> imageValue = nullptr
    )
        : baseColor(r, g, b),
        shininess(shininessValue),
        specularStrength(specularStrengthValue),
        image(std::move(imageValue))
    {
    }
};
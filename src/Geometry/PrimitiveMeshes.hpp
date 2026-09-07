#pragma once

#include <cstdint>
#include <memory>

#include "Geometry/Mesh.hpp"

Mesh createCubeMesh(float halfExtent = 1.0f);

Mesh createUvSphereMesh(
    float radius,
    std::uint32_t latitudeSegments,
    std::uint32_t longitudeSegments
);
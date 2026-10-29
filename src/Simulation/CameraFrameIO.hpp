#pragma once

#include "Simulation/CameraFrame.hpp"
#include <filesystem>

void saveCameraFrame(
    const CameraFrame& frame,
    const std::filesystem::path& prefix
);
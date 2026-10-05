#pragma once

#include "Camera/Camera.hpp"
#include "Simulation/SensorRig.hpp"
#include "Simulation/Trajectory.hpp"
#include "Transform/Pose.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>

class Scenario
{
public:
    const std::filesystem::path& worldPath() const noexcept;

    const SensorRig& rig(const std::string& name) const;
    const Trajectory& trajectory(const std::string& name) const;
    
private:
    std::filesystem::path worldPath_;    
    std::unordered_map<std::string, SensorRig> rigs_;
    std::unordered_map<std::string, Trajectory> trajs_;

    friend Scenario loadScenario(const std::filesystem::path& path);
};

Scenario loadScenario(const std::filesystem::path& path);

#pragma once

#include "Camera/Camera.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>

class Scenario
{
public:
    const std::filesystem::path& worldPath() const noexcept;

    const Camera& camera(const std::string& name) const;

private:
    std::filesystem::path worldPath_;    
    std::unordered_map<std::string, Camera> cameras_;

    friend Scenario loadScenario(const std::filesystem::path& path);
};

Scenario loadScenario(const std::filesystem::path& path);

#include "Scenario.hpp"

#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>

namespace {

Eigen::Vector3f readVector3(const nlohmann::json& value)
{
    return {
        value.at(0).get<float>(),
        value.at(1).get<float>(),
        value.at(2).get<float>()
    };
}

Eigen::Quaternionf readQuaternion(const nlohmann::json& value)
{
    // Scenario convention: [w, x, y, z]
    return {
        value.at(0).get<float>(),
        value.at(1).get<float>(),
        value.at(2).get<float>(),
        value.at(3).get<float>()
    };
}

Camera readCamera(const nlohmann::json& value)
{
    const auto& intrinsics = value.at("intrinsics");
    Camera camera(
        {
            intrinsics.at("width").get<int>(),
            intrinsics.at("height").get<int>(),
            intrinsics.at("fx").get<float>(),
            intrinsics.at("fy").get<float>(),
            intrinsics.at("cx").get<float>(),
            intrinsics.at("cy").get<float>()
        },
        value.at("near").get<float>(),
        value.at("far").get<float>()
    );
    camera.setPosition(readVector3(value.at("position")));
    camera.setRotation(readQuaternion(value.at("orientation")));

    return camera;
}

SensorRig readRig(const nlohmann::json& value)
{
    const auto& pose = value.at("pose");
    SensorRig rig(        
        readVector3(pose.at("position")),
        readQuaternion(pose.at("orientation"))
    );
    for (auto& camJson: value.at("cameras")) {
        const auto& intrinsics = camJson.at("intrinsics");
        const auto& localPose = value.at("pose");

        Camera camera(
        {
                intrinsics.at("width").get<int>(),
                intrinsics.at("height").get<int>(),
                intrinsics.at("fx").get<float>(),
                intrinsics.at("fy").get<float>(),
                intrinsics.at("cx").get<float>(),
                intrinsics.at("cy").get<float>()
            },
            camJson.at("near").get<float>(),
            camJson.at("far").get<float>()
        );
        
        rig.addCamera(
            camJson.at("name").get<std::string>(),
            {
                camera,
                readVector3(localPose.at("position")),
                readQuaternion(localPose.at("orientation"))
            }
        );
    }
    return rig;
}

}


const std::filesystem::path& Scenario::worldPath() const noexcept
{
    return worldPath_;
}


// const Camera& Scenario::camera(const std::string& name) const
// {
//     auto it = cameras_.find(name);    
//     if (it == cameras_.end()) {
//         throw std::runtime_error("No camera name: " + name);
//     }

//     return it->second;
// }

const SensorRig& Scenario::rig(const std::string& name) const
{
    auto it = rigs_.find(name);    
    if (it == rigs_.end()) {
        throw std::runtime_error("No rig name: " + name);
    }

    return it->second;

}



Scenario loadScenario(const std::filesystem::path& path)
{
    std::ifstream file(path);

    if (!file) {
        throw std::runtime_error("Failed to open scenario: " + path.string());
    }

    nlohmann::json json;
    file >> json;

    Scenario scenario;

    scenario.worldPath_ = json.at("rigs").get<std::string>();

    for (const auto& rigJson : json.at("cameras")) {
        std::string name = rigJson.at("name").get<std::string>();
        auto [it, inserted] = scenario.rigs_.emplace(std::move(name), readRig(rigJson));  
        if (!inserted) {
            throw std::runtime_error("Duplicate camera name: " + it->first);
        }
    }

    return scenario;

}
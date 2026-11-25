#include "CameraFrameIO.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <cstddef>
#include <nlohmann/json.hpp>

void saveCameraFrame(
    const CameraFrame& frame,
    const std::filesystem::path& prefix)
{   
    if (!prefix.parent_path().empty()) {
        std::filesystem::create_directories(prefix.parent_path());
    }

    auto rgbPath = prefix;
    rgbPath += ".rgb";

    std::ofstream rgbFile(rgbPath, std::ios::binary);
    if (!rgbFile) {
        throw std::runtime_error("Failed to open RGB file");
    }

    for (int y = 0; y < frame.rgb.height(); ++y) {
        const auto row = frame.rgb.row(static_cast<std::size_t>(y));

        rgbFile.write(
            reinterpret_cast<const char*>(row.data()),
            static_cast<std::streamsize>(row.size())
        );
    }

    if (!rgbFile) {
        throw std::runtime_error("Failed to write RGB file");
    }

    auto depthPath = prefix;
    depthPath += ".depth";

    std::ofstream depthFile(depthPath, std::ios::binary);
    if (!depthFile) {
        throw std::runtime_error("Failed to open RGB file");
    }

    for (int y = 0; y < frame.rawDepth.height(); ++y) {
        const auto row = frame.rawDepth.row(static_cast<std::size_t>(y));

        depthFile.write(
            reinterpret_cast<const char*>(row.data()),
            static_cast<std::streamsize>(row.size_bytes())
        );
    }

    if (!depthFile) {
        throw std::runtime_error("Failed to write RGB file");
    }

    
    // Save frame metadata
    auto jsonPath = prefix;
    jsonPath += ".json";

    auto originToString = [](ImageOrigin origin) {
        return origin == ImageOrigin::BottomLeft
            ? "BottomLeft"
            : "TopLeft";
    };

    nlohmann::json metadata = {
        {"frameIndex", frame.frameIndex},
        {"timestamp", frame.timestamp},
        {"rgb", {
            {"width", frame.rgb.width()},
            {"height", frame.rgb.height()},
            {"format", "RGB8"},
            {"origin", originToString(frame.rgb.origin())}
        }},
        {"depth", {
            {"width", frame.rawDepth.width()},
            {"height", frame.rawDepth.height()},
            {"format", "Float32"},
            {"origin", originToString(frame.rawDepth.origin())},
            {"representation", "OpenGL"}
        }}
    };

    std::ofstream jsonFile(jsonPath);
    if (!jsonFile) {
        throw std::runtime_error("Failed to open JSON file");
    }

    jsonFile << metadata.dump(2) << '\n';

    if (!jsonFile) {
        throw std::runtime_error("Failed to write JSON file");
    }

}


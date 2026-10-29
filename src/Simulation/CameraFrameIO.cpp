#include "CameraFrameIO.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <cstddef>


void saveCameraFrame(
    const CameraFrame& frame,
    const std::filesystem::path& prefix)
{   
    if (!prefix.parent_path().empty()) {
        std::filesystem::create_directories(prefix.parent_path());
    }

    auto rgbPath = prefix;
    rgbPath += ".rgb";

    std::ofstream file(rgbPath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open RGB file");
    }

    for (int y = 0; y < frame.rgb.height(); ++y) {
        const auto row = frame.rgb.row(static_cast<std::size_t>(y));

        file.write(
            reinterpret_cast<const char*>(row.data()),
            static_cast<std::streamsize>(row.size())
        );
    }

    if (!file) {
        throw std::runtime_error("Failed to write RGB file");
    }
}


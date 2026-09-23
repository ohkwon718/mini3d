#include "PointCloud.hpp"

#include <fstream>

PointCloud::PointCloud(std::vector<Eigen::Vector3f> positions)
    : positions_(std::move(positions))
{
}


void PointCloud::savePointCloudPly(const std::string& path)
{
    std::ofstream file(path);

    if (!file) {
        throw std::runtime_error(
            "Failed to open point cloud file: " + path
        );
    }

    file << "ply\n";
    file << "format ascii 1.0\n";
    file << "element vertex " << positions_.size() << '\n';
    file << "property float x\n";
    file << "property float y\n";
    file << "property float z\n";
    file << "end_header\n";

    for (const auto& pos : positions_) {
        file << pos.x() << ' '
             << pos.y() << ' '
             << pos.z() << '\n';
    }
}
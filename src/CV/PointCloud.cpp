#include "PointCloud.hpp"

#include <utility>
#include <fstream>

PointCloud::PointCloud(std::vector<Eigen::Vector3f> positions)
    : positions_(std::move(positions))
{
}

PointCloud PointCloud::fromDepth(
        const std::vector<float>& depth,
        int width,
        int height,
        const Camera& camera)
{
    std::vector<Eigen::Vector3f> points;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const std::size_t index = static_cast<std::size_t>(y) * width + x;
            const float raw = depth[index];

            if (raw >= 1.0f) {
                continue;
            }

            const float depth = camera.depthToMetric(raw);
            const float u = static_cast<float>(x);
            const float v = static_cast<float>(height - 1 - y);

            const Eigen::Vector3f point =
                camera.unproject({u, v}, depth);

            points.push_back(point);
        }
    }
    return PointCloud(std::move(points));
}



void PointCloud::savePointCloudPly(const std::string& path) const
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
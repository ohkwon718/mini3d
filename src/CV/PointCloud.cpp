#include "PointCloud.hpp"

#include <utility>
#include <fstream>

PointCloud::PointCloud(
    std::vector<Eigen::Vector3f> positions,
    std::vector<Eigen::Vector3f> colors)
    : positions_(std::move(positions)),
      colors_(std::move(colors))
{
    if (!colors_.empty() &&
        colors_.size() != positions_.size()) {
        throw std::invalid_argument(
            "PointCloud colors must match positions size"
        );
    }    
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

            const Eigen::Vector3f point = camera.unproject({u, v}, depth);

            points.push_back(point);
        }
    }
    return PointCloud(std::move(points));
}



PointCloud PointCloud::fromRgbd(
    const std::vector<float>& depth,
    const RgbImage& rgb,
    const Camera& camera)
{   
    if ( depth.size() != rgb.width*rgb.height || rgb.data.size() != 3*rgb.width*rgb.height) {
        throw std::invalid_argument( "image and depth sizes are unmatched" );
    }

    std::vector<Eigen::Vector3f> points;
    std::vector<Eigen::Vector3f> rgbs;

    for (int y = 0; y < rgb.height; ++y) {
        for (int x = 0; x < rgb.width; ++x) {
            const std::size_t index = static_cast<std::size_t>(y) * rgb.width + x;
            const float raw = depth[index];

            if (raw >= 1.0f) {
                continue;
            }

            const float depth = camera.depthToMetric(raw);
            const float u = static_cast<float>(x);
            const float v = static_cast<float>(rgb.height - 1 - y);

            const Eigen::Vector3f point = camera.unproject({u, v}, depth);

            points.push_back(point);
            
            rgbs.push_back({rgb.data[index*3], rgb.data[index*3+1], rgb.data[index*3+2]});
        }
    }
    return PointCloud(std::move(points), std::move(rgbs));

}


void PointCloud::savePointCloudPly(const std::string& path) const
{
    std::ofstream file(path);   

    if (!file) {
        throw std::runtime_error(
            "Failed to open point cloud file: " + path
        );
    }

    bool isRGB = colors_.size() > 0;
    if (isRGB) {
        if(positions_.size() != colors_.size()) {
            throw std::runtime_error(
                "The sizes of postions and colors are not equal"
            );
        }
    }

    file << "ply\n";
    file << "format ascii 1.0\n";
    file << "element vertex " << positions_.size() << '\n';
    file << "property float x\n";
    file << "property float y\n";
    file << "property float z\n";
    if (isRGB) {
        file << "property uchar red\n";
        file << "property uchar green\n";
        file << "property uchar blue\n";
    }
    file << "end_header\n";

    for (size_t i = 0 ; i < positions_.size() ; ++i) {
        auto pos = positions_[i];
        file << pos.x() << ' '
             << pos.y() << ' '
             << pos.z();
        if (isRGB) {
            auto col = colors_[i];
            file << ' ' << col.x();
            file << ' ' << col.y();
            file << ' ' << col.z();
        }
        file << '\n';
    }    
}
#include "PointCloud.hpp"

#include <utility>
#include <fstream>

PointCloud::PointCloud(
    std::vector<Eigen::Vector3f> positions,
    std::vector<Color3u> colors)
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
    if ( width <= 0 or height <= 0) {
        throw std::invalid_argument( "width and height must be positive" );
    }
    if ( depth.size() != static_cast<std::size_t>(width)*static_cast<std::size_t>(height)) {
        throw std::invalid_argument( "depth sizes is not matched" );
    }
    if (camera.intrinsics().width != width || camera.intrinsics().height != height)
    {
        throw std::invalid_argument( "camera intrinsics info and the size are unmatched" );
    }

    std::vector<Eigen::Vector3f> points;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const std::size_t index = static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x);
            const float raw = depth[index];

            if (raw >= 1.0f) {
                continue;
            }

            const float metricDepth = camera.depthToMetric(raw);
            const float u = static_cast<float>(x);
            const float v = static_cast<float>(height - 1 - y);

            const Eigen::Vector3f point = camera.unproject({u, v}, metricDepth);

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
    if ( rgb.width <= 0 or rgb.height <= 0 ) {
        throw std::invalid_argument( "width and height must be positive" );
    }
    const std::size_t width = static_cast<std::size_t>(rgb.width);
    const std::size_t height = static_cast<std::size_t>(rgb.height);
    const std::size_t pixelCount = width * height;
    if ( depth.size() != pixelCount || rgb.data.size() != 3*pixelCount ) {
        throw std::invalid_argument( "image and depth sizes are unmatched" );
    }
    if ( camera.intrinsics().width != rgb.width || camera.intrinsics().height != rgb.height )
    {
        throw std::invalid_argument( "camera intrinsics info and rgb size are unmatched" );
    }

    std::vector<Eigen::Vector3f> points;
    std::vector<Color3u> colors;

    for (std::size_t y = 0; y < height; ++y) {
        for (std::size_t x = 0; x < width; ++x) {
            const std::size_t index = y * width + x;
            const float raw = depth[index];

            if (raw >= 1.0f) {
                continue;
            }

            const float depth = camera.depthToMetric(raw);
            const float u = static_cast<float>(x);
            const float v = static_cast<float>(height - 1 - y);

            const Eigen::Vector3f point = camera.unproject({u, v}, depth);

            points.push_back(point);
            
            colors.push_back({rgb.data[index*3], rgb.data[index*3+1], rgb.data[index*3+2]});
        }
    }
    return PointCloud(std::move(points), std::move(colors));

}


void PointCloud::savePointCloudPly(const std::string& path) const
{
    std::ofstream file(path);   

    if (!file) {
        throw std::runtime_error(
            "Failed to open point cloud file: " + path
        );
    }

    const bool hasColors = !colors_.empty();

    file << "ply\n";
    file << "format ascii 1.0\n";
    file << "element vertex " << positions_.size() << '\n';
    file << "property float x\n";
    file << "property float y\n";
    file << "property float z\n";
    if (hasColors) {
        file << "property uchar red\n";
        file << "property uchar green\n";
        file << "property uchar blue\n";
    }
    file << "end_header\n";

    for (size_t i = 0 ; i < positions_.size() ; ++i) {
        const auto& pos = positions_[i];
        file << pos.x() << ' '
             << pos.y() << ' '
             << pos.z();
        if (hasColors) {
            const auto& col = colors_[i];
            file << ' ' << static_cast<unsigned int>(col[0]);
            file << ' ' << static_cast<unsigned int>(col[1]);
            file << ' ' << static_cast<unsigned int>(col[2]);
        }
        file << '\n';
    }    
}
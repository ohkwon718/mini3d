#pragma once

#include <vector>
#include <Eigen/Dense>
#include <Camera/Camera.hpp>
#include <CV/RgbImage.hpp>

using Color3u = std::array<std::uint8_t, 3>;

class PointCloud
{
public:
    explicit PointCloud(
        std::vector<Eigen::Vector3f> positions, 
        std::vector<Color3u> colors = {}
    );
    
    static PointCloud fromDepth(
        const std::vector<float>& depth,
        int depth_width,
        int depth_height,
        const Camera& camera
    );

    static PointCloud fromRgbd(
        const std::vector<float>& depth,
        const RgbImage& rgb,        
        const Camera& camera
    );


    void savePointCloudPly(const std::string& path) const;

private:
    std::vector<Eigen::Vector3f> positions_;    
    std::vector<Color3u> colors_;    
    
};
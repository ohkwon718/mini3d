#pragma once

#include <array>
#include <string>
#include <vector>
#include <cstdint>
#include <Eigen/Dense>
#include "Camera/Camera.hpp"
#include "core/RgbImage.hpp"
#include "core/DepthImage.hpp"



using Color3u = std::array<std::uint8_t, 3>;

class PointCloud
{
public:
    explicit PointCloud(
        std::vector<Eigen::Vector3f> positions, 
        std::vector<Color3u> colors = {}
    );
    
    static PointCloud fromDepth(
        const DepthImage& depth,
        const Camera& camera
    );

    static PointCloud fromRgbd(
        const DepthImage& depth,
        const RgbImage& rgb,        
        const Camera& camera
    );


    void savePointCloudPly(const std::string& path) const;

private:
    std::vector<Eigen::Vector3f> positions_;    
    std::vector<Color3u> colors_;    
    
};
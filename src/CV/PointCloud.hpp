#pragma once

#include <vector>
#include <Eigen/Dense>
#include <Camera/Camera.hpp>
#include <CV/RgbImage.hpp>

class PointCloud
{
public:
    explicit PointCloud(
        std::vector<Eigen::Vector3f> positions, 
        std::vector<Eigen::Vector3f> color = {}
    );
    
    static PointCloud fromDepth(
        const std::vector<float>& depth,
        int width,
        int height,
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
    std::vector<Eigen::Vector3f> colors_;
};
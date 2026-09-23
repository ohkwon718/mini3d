#pragma once

#include <vector>
#include <Eigen/Dense>
#include <Camera/Camera.hpp>

class PointCloud
{
public:
    explicit PointCloud(std::vector<Eigen::Vector3f> positions);

    static PointCloud fromDepth(
        const std::vector<float>& depth,
        int width,
        int height,
        const Camera& camera
    );

    void savePointCloudPly(const std::string& path) const;

private:
    std::vector<Eigen::Vector3f> positions_;

};
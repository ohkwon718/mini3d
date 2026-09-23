#pragma once

#include <vector>
#include <Eigen/Dense>


class PointCloud
{
public:
    explicit PointCloud(std::vector<Eigen::Vector3f> points);

    void savePointCloudPly(const std::string& path);


private:
    std::vector<Eigen::Vector3f> positions_;

};
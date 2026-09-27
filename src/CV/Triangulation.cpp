#include "Triangulation.hpp"

Eigen::Vector3f triangulateLinearSvd(
    const Camera& cameraA,
    const Eigen::Vector2f& pixelA,
    const Camera& cameraB,
    const Eigen::Vector2f& pixelB)
{
    const auto P1 = cameraA.cameraMatrix();
    const auto P2 = cameraB.cameraMatrix();
    
    Eigen::Matrix4f A;
    
    A.row(0) = pixelA.x() * P1.row(2) - P1.row(0);    
    A.row(1) = pixelA.y() * P1.row(2) - P1.row(1);
    A.row(2) = pixelB.x() * P2.row(2) - P2.row(0);
    A.row(3) = pixelB.y() * P2.row(2) - P2.row(1);
    
    Eigen::JacobiSVD<Eigen::Matrix4f> svd(A, Eigen::ComputeFullV);
    Eigen::Vector4f res = svd.matrixV().col(3);    
    if (std::abs(res.w()) < 1e-6f) {
        throw std::runtime_error("Triangulation produced point at infinity");
    }

    return res.head(3) / res(3);
}

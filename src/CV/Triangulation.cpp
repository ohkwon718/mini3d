#include "Triangulation.hpp"

#include <iostream>

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


Eigen::Vector3f triangulateClosestRays(
    const Camera& cameraA,
    const Eigen::Vector2f& pixelA,
    const Camera& cameraB,
    const Eigen::Vector2f& pixelB)
{    
    const auto posA = cameraA.position().cast<double>();;
    const auto posB = cameraB.position().cast<double>();;
    const auto dirA = (cameraA.unproject(pixelA, 1).cast<double>() - posA).normalized();
    const auto dirB = (cameraB.unproject(pixelB, 1).cast<double>() - posB).normalized();
    double dAdB = dirA.dot(dirB);
    const Eigen::Vector3d vecAB = posB - posA;
    double b1 = vecAB.dot(dirA);
    double b2 = vecAB.dot(dirB);
    double denom = 1-dAdB*dAdB;
    if (std::abs(denom) < 1e-12) {
        throw std::runtime_error(
            "Cannot triangulate nearly parallel rays"
        );
    }
    double tA = (b1 - dAdB*b2)/denom;
    double tB = (dAdB*b1 - b2)/denom;
    const Eigen::Vector3d pointA = posA + tA * dirA;
    const Eigen::Vector3d pointB = posB + tB * dirB;

    return (0.5 * (pointA + pointB)).cast<float>();    
}
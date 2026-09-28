#include "Triangulation.hpp"



Eigen::Vector3f triangulateLinearSvd(
    std::span<const CameraObservation> observations)
{
    if (observations.size() < 2) {
        throw std::invalid_argument(
            "Triangulation requires at least two observations"
        );
    }
    const Eigen::Index numObservations = static_cast<Eigen::Index>(observations.size());
    Eigen::MatrixXf A(2 * numObservations, 4);    
    
    for (Eigen::Index i = 0; i < numObservations; ++i) {
        const auto& obs = observations[static_cast<std::size_t>(i)];
        if (obs.camera == nullptr) {
            throw std::invalid_argument(
                "Camera observation contains null camera"
            );
        }
        const auto& P = obs.camera->cameraMatrix();
        const auto& pixel = obs.pixel;
        
        A.row(2*i  ) = pixel.x() * P.row(2) - P.row(0);    
        A.row(2*i+1) = pixel.y() * P.row(2) - P.row(1);
    }

    Eigen::JacobiSVD<Eigen::MatrixXf> svd(A, Eigen::ComputeFullV);
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
    const Eigen::Vector3d posA = cameraA.position().cast<double>();;
    const Eigen::Vector3d posB = cameraB.position().cast<double>();;
    const Eigen::Vector3d dirA = (cameraA.unproject(pixelA, 1).cast<double>() - posA).normalized();
    const Eigen::Vector3d dirB = (cameraB.unproject(pixelB, 1).cast<double>() - posB).normalized();
    double dAdB = dirA.dot(dirB);
    const Eigen::Vector3d vecAB = posB - posA;
    double b1 = vecAB.dot(dirA);
    double b2 = vecAB.dot(dirB);
    double denom = 1-dAdB*dAdB;
    double tA = (b1 - dAdB*b2)/denom;
    double tB = (dAdB*b1 - b2)/denom;
    return 0.5f * (posA + tA*dirA + posB + tB*dirB).cast<float>();;
}


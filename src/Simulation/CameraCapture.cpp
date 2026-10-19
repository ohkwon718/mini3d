#include "CameraCapture.hpp"
#include <limits>
#include <utility>

#include "core/DepthImage.hpp"

namespace
{

void convertToMetricDepth(DepthImage& depth, const Camera& camera)
{
    const std::size_t height = static_cast<std::size_t>(depth.height());

    for (std::size_t y = 0; y < height; ++y) {
        auto row = depth.row(y);

        for (float& value : row) {
            if (value >= 1.0f) {
                value = std::numeric_limits<float>::quiet_NaN();
            }
            else {
                value = camera.depthToMetric(value);
            }
        }
    }
}

}

CameraFrame captureCameraFrame(
    const RenderTarget& target,
    const Camera& camera,
    std::size_t frameIndex,
    double timestamp)
{   
    DepthImage depth = target.readDepth();
    convertToMetricDepth(depth, camera);    

    return CameraFrame{
        frameIndex, 
        timestamp,
        Pose{
            camera.position(),
            camera.rotation(),
        },
        camera.intrinsics(),
        target.readRgb(),
        std::move(depth)
    };
}



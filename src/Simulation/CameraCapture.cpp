#include "CameraCapture.hpp"
#include <limits>
#include <utility>

#include "core/DepthImage.hpp"


CameraFrame captureCameraFrame(
    const RenderTarget& target,
    const Camera& camera,
    std::size_t frameIndex,
    double timestamp)
{   

    return CameraFrame{
        frameIndex, 
        timestamp,
        Pose{
            camera.position(),
            camera.rotation(),
        },
        camera.intrinsics(),
        target.readRgb(),
        target.readDepth()
    };
}



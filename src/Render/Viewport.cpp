#include "Viewport.hpp"

#include <algorithm>
#include <stdexcept>

Viewport fitViewport(
    int framebufferWidth,
    int framebufferHeight,
    int cameraImageWidth,
    int cameraImageHeight)
{
    if (framebufferWidth <= 0 || framebufferHeight <= 0 ||
        cameraImageWidth <= 0 || cameraImageHeight <= 0) {
        throw std::invalid_argument("Viewport dimensions must be positive");
    }

    const float scaleX = static_cast<float>(framebufferWidth) / static_cast<float>(cameraImageWidth);
    const float scaleY = static_cast<float>(framebufferHeight) / static_cast<float>(cameraImageHeight);
    const float scale = std::min({1.0f, scaleX, scaleY});

    const int viewportWidth = static_cast<int>(static_cast<float>(cameraImageWidth) * scale);
    const int viewportHeight = static_cast<int>(static_cast<float>(cameraImageHeight) * scale);

    const int x = (framebufferWidth - viewportWidth) / 2;
    const int y = (framebufferHeight - viewportHeight) / 2;

    return {
        x,
        y,
        viewportWidth,
        viewportHeight
    };
}
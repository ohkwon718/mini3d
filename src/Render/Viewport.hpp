#pragma once

struct Viewport
{
    int x;
    int y;
    int width;
    int height;
};

Viewport fitViewport(
    int framebufferWidth,
    int framebufferHeight,
    int imageWidth,
    int imageHeight
);
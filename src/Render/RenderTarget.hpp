#pragma once

#include <glad/gl.h>
#include <vector>
#include <cstdint>
#include <Render/Viewport.hpp>
#include <CV/RgbImage.hpp>

class RenderTarget
{
public:
    RenderTarget(int width, int height);
    ~RenderTarget();

    RenderTarget(const RenderTarget&) = delete;
    RenderTarget& operator=(const RenderTarget&) = delete;

    RenderTarget(RenderTarget&& other) noexcept;
    RenderTarget& operator=(RenderTarget&& other) noexcept;

    void bind() const;    
    RgbImage readRgb() const;
    std::vector<float> readDepth() const;

    static void bindDefault(int width, int height);
    static void bindDefault(const Viewport& viewport);

    const int width() const;
    const int height() const;
    
private:
    void release() noexcept;

    int width_;
    int height_;

    GLuint framebuffer_{0};
    GLuint colorTexture_{0};
    GLuint depthTexture_{0};
};
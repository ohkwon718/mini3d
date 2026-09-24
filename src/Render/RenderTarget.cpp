#include "RenderTarget.hpp"
#include <stdexcept>


RenderTarget::RenderTarget(int width, int height)
    : width_(width),
      height_(height)
{
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("width and height should be positive");
    }   

    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    // Color
    glGenTextures(1, &colorTexture_);
    glBindTexture(GL_TEXTURE_2D, colorTexture_);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB8,
        width_,
        height_,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        nullptr
    );

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        colorTexture_,
        0
    );

    // Depth
    glGenTextures(1, &depthTexture_);
    glBindTexture(GL_TEXTURE_2D, depthTexture_);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT32F,
        width_,
        height_,
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        nullptr
    );

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        depthTexture_,
        0
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)
        != GL_FRAMEBUFFER_COMPLETE)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        release();        
        throw std::runtime_error("Framebuffer is incomplete");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

RenderTarget::~RenderTarget()
{
    release();
}

RenderTarget::RenderTarget(RenderTarget&& other) noexcept
    : width_(other.width_),
    height_(other.height_),
    framebuffer_(other.framebuffer_), 
    colorTexture_(other.colorTexture_),
    depthTexture_(other.depthTexture_)
{
    other.framebuffer_ = 0;
    other.colorTexture_ = 0;
    other.depthTexture_ = 0;
    other.width_ = 0;
    other.height_ = 0;
}

RenderTarget& RenderTarget::operator=(RenderTarget&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }    
    release();
    
    framebuffer_ = other.framebuffer_;
    colorTexture_ = other.colorTexture_;
    depthTexture_ = other.depthTexture_; 
    width_ = other.width_;
    height_ = other.height_;   
    other.framebuffer_ = 0;
    other.colorTexture_ = 0;
    other.depthTexture_ = 0;
    other.width_ = 0;
    other.height_ = 0;
    
    
    return *this;
}

void RenderTarget::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glViewport(0, 0, width_, height_);
}

RgbImage RenderTarget::readRgb() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    std::vector<std::uint8_t> pixels(
        static_cast<std::size_t>(width_) *
        static_cast<std::size_t>(height_) *
        3
    );

    GLint previousAlignment;
    glGetIntegerv(GL_PACK_ALIGNMENT, &previousAlignment);    
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    glReadPixels(
        0,
        0,
        width_,
        height_,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixels.data()
    );

    glPixelStorei(GL_PACK_ALIGNMENT, previousAlignment);
    
    return RgbImage{width_, height_, std::move(pixels)};
}

std::vector<float> RenderTarget::readDepth() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);    
    std::vector<float> depth(
        static_cast<std::size_t>(width_) *
        static_cast<std::size_t>(height_)
    );
    glReadPixels(
        0,
        0,
        width_,
        height_,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        depth.data()
    );

    return depth;
}


void RenderTarget::bindDefault(int width, int height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    
}


void RenderTarget::bindDefault(const Viewport& viewport)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(
        viewport.x,
        viewport.y,
        viewport.width,
        viewport.height
    );
}


void RenderTarget::release() noexcept
{
    if (depthTexture_ != 0) glDeleteTextures(1, &depthTexture_);
    if (colorTexture_ != 0) glDeleteTextures(1, &colorTexture_);    
    if (framebuffer_ != 0) glDeleteFramebuffers(1, &framebuffer_);
    framebuffer_ = 0;
    colorTexture_ = 0;
    depthTexture_ = 0;
}


int RenderTarget::width() const
{
    return width_;
}

int RenderTarget::height() const
{
    return height_;
}

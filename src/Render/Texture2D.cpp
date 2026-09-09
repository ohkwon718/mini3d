#include "Texture2D.hpp"
#include <glad/gl.h>

Texture2D::Texture2D(
    int width,
    int height,
    const unsigned char* pixels
)
{    
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

Texture2D::~Texture2D() 
{
    if (id_ != 0) {
        glDeleteTextures(1, &id_);
    }
}

Texture2D::Texture2D(Texture2D&& other) noexcept 
    : id_(other.id_)
{   
    other.id_ = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept 
{
    if (this == &other) {
        return *this;
    }

    if (id_ != 0) {
        glDeleteTextures(1, &id_);
    }

    id_ = other.id_;
    other.id_ = 0;

    return *this;

}

void Texture2D::bind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_);
}
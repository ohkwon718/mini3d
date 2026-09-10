#include "Image.hpp"

#include <stdexcept>
#include <utility>

Image::Image(
    int width,
    int height,
    std::vector<unsigned char> pixels
): width_(width), height_(height), pixels_(std::move(pixels))
{
    if (width <= 0)
    {
        throw std::invalid_argument( "received invalid width" );
    }
    if (height <= 0)
    {
        throw std::invalid_argument( "received invalid height" );
    }
    if (pixels_.size() != 
            static_cast<std::size_t>(width) * 
            static_cast<std::size_t>(height) * 4)
    {
        throw std::invalid_argument( "received invalid sizes" );
    }    
}

int Image::width() const
{
    return width_;
}

int Image::height() const
{
    return height_;
}

const std::vector<unsigned char>& Image::pixels() const
{
    return pixels_;
}
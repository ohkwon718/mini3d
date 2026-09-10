#include "ImageLoader.hpp"
#include <utility>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Image loadImage(const std::filesystem::path& path)
{   
    int width;
    int height;

    auto deleter = [](unsigned char* ptr) {
        stbi_image_free(ptr);
    };

    std::unique_ptr<unsigned char, decltype(deleter)> pixels{
        stbi_load(path.c_str(), &width, &height, nullptr, STBI_rgb_alpha),
        deleter
    };

    if (!pixels) {
        throw std::runtime_error(
            stbi_failure_reason()
        );        
    }
    std::size_t size = static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4;

    std::vector<unsigned char> data(
        pixels.get(),
        pixels.get() + size
    );
    
    return Image(width, height, std::move(data));
}
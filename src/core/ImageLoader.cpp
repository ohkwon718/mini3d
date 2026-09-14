#include "ImageLoader.hpp"
#include <utility>
#include <limits>
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


Image loadImage(std::span<const std::byte> encodedData)
{
    if (encodedData.empty()) {
        throw std::invalid_argument("Image data is empty");
    }
    if (encodedData.size() > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::invalid_argument("Encoded image is too large");
    }
    
    int width;
    int height;

    auto* rawPixels = stbi_load_from_memory(
        reinterpret_cast<const stbi_uc*>(encodedData.data()),
        static_cast<int>(encodedData.size()),
        &width,
        &height,
        nullptr,
        STBI_rgb_alpha
    );       

    auto deleter = [](unsigned char* ptr) {
        stbi_image_free(ptr);
    };
    std::unique_ptr<unsigned char, decltype(deleter)> pixels{
        rawPixels,
        deleter
    };
    
    if (!pixels) {
        throw std::runtime_error(stbi_failure_reason());
    }

    const std::size_t size = static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4;

    std::vector<unsigned char> data(
        pixels.get(),
        pixels.get() + size
    );

    return Image(
        width,
        height,
        std::move(data)
    );

}
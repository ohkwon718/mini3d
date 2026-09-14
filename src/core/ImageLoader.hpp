#pragma once

#include <filesystem>
#include <span>
#include <cstddef>
#include "Image.hpp"

Image loadImage(const std::filesystem::path& path);


Image loadImage(std::span<const std::byte> encodedData);
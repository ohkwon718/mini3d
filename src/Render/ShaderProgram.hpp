#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <Eigen/Dense>

class ShaderProgram
{
public:
    ShaderProgram(const char* vertexSource,
                  const char* fragmentSource);

    static ShaderProgram fromFiles(
        const std::filesystem::path& vertexPath,
        const std::filesystem::path& fragmentPath
    );

    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    void use() const;

    void setMat4(const char* name, const Eigen::Matrix4f& matrix) const;
    void setVec3(const char* name, const Eigen::Vector3f& value) const;
    void setFloat(const char* name, float value) const;
    void setInt(const char* name, int value) const;

private:
    int uniformLocation(const char* name) const;
    
    mutable std::unordered_map<std::string, int> uniformLocations_;

    unsigned int id_{0};

};
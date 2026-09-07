#pragma once

#include <Eigen/Dense>

class ShaderProgram
{
public:
    ShaderProgram(const char* vertexSource,
                  const char* fragmentSource);

    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    void use() const;

    void setMat4(const char* name, const Eigen::Matrix4f& matrix) const;
    void setVec3(const char* name, const Eigen::Vector3f& value) const;
    void setFloat(const char* name, float value) const;

private:
    unsigned int id_{0};

};
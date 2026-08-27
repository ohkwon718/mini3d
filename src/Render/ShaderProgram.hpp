#pragma once

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

private:
    unsigned int id_{0};

};
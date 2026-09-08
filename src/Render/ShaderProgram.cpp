#include "ShaderProgram.hpp"
#include <glad/gl.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

namespace {

    class ShaderHandle
    {
    public:
        explicit ShaderHandle(GLuint id) : id_(id) {}
        ~ShaderHandle() 
        { 
            if (id_ != 0) {
                glDeleteShader(id_); 
            }
        }

        ShaderHandle(const ShaderHandle&) = delete;
        ShaderHandle& operator=(const ShaderHandle&) = delete;

        ShaderHandle(ShaderHandle&& other) noexcept 
            : id_(other.id_)
        {
            other.id_ = 0;            
        }

        ShaderHandle& operator=(ShaderHandle&& other) noexcept
        {
            if (this == &other) {
                return *this;
            }
        
            if (id_ != 0) {
                glDeleteShader(id_);
            }
            id_ = other.id_;
            other.id_ = 0;

            return *this;
        }

        GLuint id() const 
        {
            return id_;
        }

    private:
        GLuint id_{0};
    };

    ShaderHandle compileShader(GLenum type, const char* source)
    {
        ShaderHandle shader{glCreateShader(type)};

        glShaderSource(shader.id(), 1, &source, nullptr);
        glCompileShader(shader.id());

        GLint success = GL_FALSE;
        glGetShaderiv(shader.id(), GL_COMPILE_STATUS, &success);

        if (success == GL_FALSE) {
            char infoLog[512];
            glGetShaderInfoLog(shader.id(), 512, nullptr, infoLog);

            throw std::runtime_error(infoLog);
        }

        return shader;
    }

    class ProgramHandle
    {
    public:
        explicit ProgramHandle(GLuint id) : id_(id) {}

        ~ProgramHandle()
        {
            if (id_ != 0) {
                glDeleteProgram(id_);
            }
        }

        ProgramHandle(const ProgramHandle&) = delete;
        ProgramHandle& operator=(const ProgramHandle&) = delete;

        GLuint id() const { return id_; }

        GLuint release()
        {
            GLuint id = id_;
            id_ = 0;
            return id;
        }

    private:
        GLuint id_{0};
    };

} // namespace

ShaderProgram::ShaderProgram(const char* vertexSource,
                             const char* fragmentSource)
{
    auto vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    auto fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);    

    ProgramHandle program(glCreateProgram());

    if (program.id() == 0) {
        throw std::runtime_error("Failed to create shader program");
    }

    glAttachShader(program.id(), vertexShader.id());    
    glAttachShader(program.id(), fragmentShader.id());

    glLinkProgram(program.id());
    
    GLint success = GL_FALSE;
    glGetProgramiv(program.id(), GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        char infoLog[512];
        glGetProgramInfoLog(program.id(), 512, nullptr, infoLog);
        throw std::runtime_error(std::string("Shader program linking failed: ") + infoLog);
    }  
    
    id_ = program.release();

}


std::string readTextFile(const std::filesystem::path& path)
{
    std::ifstream file(path);

    if (!file) {
        throw std::runtime_error(
            "Failed to open file: " + path.string()
        );
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

ShaderProgram ShaderProgram::fromFiles(
    const std::filesystem::path& vertexPath,
    const std::filesystem::path& fragmentPath)
{    
    std::string vertexSource = readTextFile(vertexPath);
    std::string fragmentSource = readTextFile(fragmentPath);

    return ShaderProgram(
        vertexSource.c_str(),
        fragmentSource.c_str()
    );
}

ShaderProgram::~ShaderProgram()
{
    if (id_ != 0) {
        glDeleteProgram(id_);
    }
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : id_(other.id_)
{
    other.id_ = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this == &other) {
        return *this;
    }

    if (id_ != 0) {
        glDeleteProgram(id_);
    }
    id_ = other.id_;
    other.id_ = 0;

    return *this;
}

void ShaderProgram::use() const
{
    glUseProgram(id_);
}

void ShaderProgram::setMat4(const char* name, const Eigen::Matrix4f& matrix) const
{
    GLint location = glGetUniformLocation(id_, name);

    glUniformMatrix4fv(
        location,
        1,
        GL_FALSE,
        matrix.data()
    );

}

void ShaderProgram::setVec3(const char* name, const Eigen::Vector3f& value) const
{
    GLint location = glGetUniformLocation(id_, name);

    glUniform3f(location, value.x(), value.y(), value.z());
}

void ShaderProgram::setFloat(const char* name, float value) const
{
    GLint location = glGetUniformLocation(id_, name);

    glUniform1f(location, value);    
}
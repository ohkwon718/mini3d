#include "GlfwRuntime.hpp"
#include <stdexcept>

GlfwRuntime::GlfwRuntime()
{
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }        
}

GlfwRuntime::~GlfwRuntime()
{
    glfwTerminate();
}


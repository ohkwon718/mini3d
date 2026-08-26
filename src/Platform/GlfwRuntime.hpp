#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <memory>

struct GLFWWindowDeleter
{
    void operator()(GLFWwindow* window) const
    {
        glfwDestroyWindow(window);
    }
    
};
using GLFWWindowPtr = std::unique_ptr<GLFWwindow, GLFWWindowDeleter>;


class GlfwRuntime 
{
public:
    GlfwRuntime();
    ~GlfwRuntime();

    GlfwRuntime(const GlfwRuntime&) = delete;
    GlfwRuntime& operator=(const GlfwRuntime&) = delete;

    

};
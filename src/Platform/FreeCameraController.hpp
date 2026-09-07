#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Camera/Camera.hpp"


class FreeCameraController
{
public:
    FreeCameraController(
        float movementSpeed,
        float mouseSensitivity
    );

    void update(
        GLFWwindow* window,
        Camera& camera,
        float deltaTime
    );

private:
    float movementSpeed_;
    float mouseSensitivity_;

    double previousMouseX_{0.0};
    double previousMouseY_{0.0};
    bool firstMouseSample_{true};
};
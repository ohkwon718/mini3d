#include "FreeCameraController.hpp"
#include "Render/RenderTarget.hpp"

FreeCameraController::FreeCameraController( float movementSpeed,
                                            float mouseSensitivity)
: movementSpeed_(movementSpeed), mouseSensitivity_(mouseSensitivity)
{

}

void FreeCameraController::update(  GLFWwindow* window,
                                    Camera& camera,
                                    float deltaTime)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        double mouseX;
        double mouseY;

        glfwGetCursorPos(window, &mouseX, &mouseY);

        if (firstMouseSample_) {
            firstMouseSample_ = false;
            previousMouseX_ = mouseX;
            previousMouseY_ = mouseY;
        }
        else {
            double deltaX = mouseX - previousMouseX_;
            double deltaY = mouseY - previousMouseY_;
            
            const float yawAngle = -static_cast<float>(deltaX) * mouseSensitivity_;
            const float pitchAngle = -static_cast<float>(deltaY) * mouseSensitivity_;

            Eigen::Quaternionf orientation = camera.rotation();
            const Eigen::Quaternionf yawRotation(
                Eigen::AngleAxisf(yawAngle, Eigen::Vector3f::UnitY())
            );
            orientation = yawRotation * orientation;
            const Eigen::Vector3f right = orientation * Eigen::Vector3f::UnitX();
            const Eigen::Quaternionf pitchRotation(
                Eigen::AngleAxisf(pitchAngle, right)
            );
            orientation = pitchRotation * orientation;
            camera.setRotation(orientation);


            previousMouseX_ = mouseX;
            previousMouseY_ = mouseY;
        }            
    }
    else {
        firstMouseSample_ = true;
    }

    const float distance = movementSpeed_ * static_cast<float>(deltaTime);

    const Eigen::Vector3f forward = camera.rotation() * Eigen::Vector3f(0.0f, 0.0f, -1.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.setPosition(camera.position() + forward * distance);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.setPosition(camera.position() - forward * distance);
    }

    const Eigen::Vector3f right = camera.rotation() * Eigen::Vector3f(1.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.setPosition(camera.position() - right * distance);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.setPosition(camera.position() + right * distance);
    }

    const Eigen::Vector3f up = camera.rotation() * Eigen::Vector3f(0.0f, 1.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.setPosition(camera.position() - up * distance);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.setPosition(camera.position() + up * distance);
    }     

    int framebufferWidth;
    int framebufferHeight;
    
    glfwGetFramebufferSize(
        window,
        &framebufferWidth,
        &framebufferHeight
    );

    if (framebufferWidth > 0 && framebufferHeight > 0) {
        RenderTarget::bindDefault(framebufferWidth, framebufferHeight);
        camera.setAspectRatio(
            static_cast<float>(framebufferWidth) / 
            static_cast<float>(framebufferHeight)
        );
    }


}
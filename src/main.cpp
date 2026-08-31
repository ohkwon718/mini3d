#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <numbers>
#include <cstdint>
#include <memory>
#include "Platform/GlfwRuntime.hpp"
#include "Render/ShaderProgram.hpp"
#include "Render/Renderer.hpp"
#include "Scene/SceneObject.hpp"
#include "Camera/Camera.hpp"


int main()
{
    GlfwRuntime glfw;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWWindowPtr window{
        glfwCreateWindow(800, 600, "mini3d", nullptr, nullptr)
    };

    if (!window) {
        return 1;
    }
    glfwMakeContextCurrent(window.get());

    if (!gladLoadGL(glfwGetProcAddress)) {
        return 1;
    }        

    ///////////////////////////////////////////

    const char* vertexSource = R"(
    #version 330 core

    layout(location = 0) in vec3 aPosition;

    uniform mat4 uModel;
    uniform mat4 uView;
    uniform mat4 uProjection;

    void main()
    {
        gl_Position =
            uProjection *
            uView *
            uModel *
            vec4(aPosition, 1.0);
    }
    )";

    const char* fragmentSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
    )";


    auto mesh = std::make_shared<const Mesh>(
        std::vector<Vertex>{
            {Eigen::Vector3f(0.0f,  0.5f, 0.0f)},
            {Eigen::Vector3f(-0.5f, -0.5f, 0.0f)},
            {Eigen::Vector3f(0.5f, -0.5f, 0.0f)}
        },
        std::vector<std::uint32_t>{0, 1, 2}
    );
    
    SceneObject object("Triangle", mesh);
    object.transform().setTranslation(
        Eigen::Vector3f(0.5f, 0.0f, 0.0f)
    );

    Renderer renderer;

    ShaderProgram shader(vertexSource, fragmentSource);
    
    Camera camera(
        std::numbers::pi_v<float> / 4.0f,
        800.0f / 600.0f,
        0.1f,
        100.0f
    );

    camera.setPosition(Eigen::Vector3f(0.0f, 0.0f, 3.0f));

    while (!glfwWindowShouldClose(window.get())) {
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.draw(object, camera, shader);

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }


    return 0;
}
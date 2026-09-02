#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <numbers>
#include <cstdint>
#include <memory>
#include <cassert>
#include "Platform/GlfwRuntime.hpp"
#include "Render/ShaderProgram.hpp"
#include "Render/Renderer.hpp"
#include "Render/RenderTarget.hpp"
#include "Scene/SceneObject.hpp"
#include "Scene/Scene.hpp"
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
    glEnable(GL_DEPTH_TEST);

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
    
    auto cube = std::make_shared<const Mesh>(
        std::vector<Vertex>{
            {Eigen::Vector3f(-1.0f, -1.0f, -1.0f)},
            {Eigen::Vector3f(-1.0f, -1.0f, 1.0f)},
            {Eigen::Vector3f(-1.0f, 1.0f, -1.0f)},
            {Eigen::Vector3f(-1.0f, 1.0f, 1.0f)},
            {Eigen::Vector3f(1.0f, -1.0f, -1.0f)},
            {Eigen::Vector3f(1.0f, -1.0f, 1.0f)},
            {Eigen::Vector3f(1.0f, 1.0f, -1.0f)},
            {Eigen::Vector3f(1.0f, 1.0f, 1.0f)}            
        },
        std::vector<std::uint32_t>{
            0, 1, 2,
            3, 1, 2,
            4, 5, 6,
            7, 5, 6,
            0, 1, 4,
            5, 1, 4,
            2, 3, 6,
            7, 3, 6,
            0, 2, 4,
            6, 2, 4,
            1, 3, 5,
            7, 3, 5            
        }
    );    

    Scene scene;
    SceneObject object("Cube", cube);
    object.transform().setTranslation(
        Eigen::Vector3f(0.0f, 0.0f, -10.0f)
    );    
    object.transform().setRotation(
        Eigen::Quaternionf(0.5f, 0.5f, 0.5f, 1.0f)
    );
    scene.addObject(std::move(object));

    Renderer renderer;

    ShaderProgram shader(vertexSource, fragmentSource);
    
    Camera camera(
        std::numbers::pi_v<float> / 4.0f,
        800.0f / 600.0f,
        0.1f,
        100.0f
    );

    camera.setPosition(Eigen::Vector3f(0.0f, 0.0f, 3.0f));

    RenderTarget target(640, 480);
    target.bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer.draw(scene, camera, shader);

    auto rgb = target.readRgb();
    auto depth = target.readDepth();
    assert(rgb.size() == 640 * 480 * 3);
    assert(depth.size() == 640 * 480);

    RenderTarget::bindDefault(800, 600);

    while (!glfwWindowShouldClose(window.get())) {        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.draw(scene, camera, shader);

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }


    return 0;
}
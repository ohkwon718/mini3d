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
    layout(location = 1) in vec3 aNormal;
    out vec3 vNormal;

    uniform mat4 uModel;
    uniform mat4 uView;
    uniform mat4 uProjection;

    void main()
    {
        gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);        
        vNormal = normalize( mat3(transpose(inverse(uModel))) * aNormal );
    }
    )";

    const char* fragmentSource = R"(
    #version 330 core

    in vec3 vNormal;        
    out vec4 FragColor;

    uniform vec3 uLightDirection;
    uniform vec3 uBaseColor;
    

    void main()
    {   
        vec3 N = normalize(vNormal);
        vec3 L = normalize(uLightDirection);
        float diffuse = max(dot(N, L), 0.0);
        float ambient = 0.15;
        float brightness = ambient + diffuse;
        
        FragColor = vec4(uBaseColor * brightness, 1.0);
    }
    )";
     
    Eigen::Vector3f lightDirection(-0.5f, -1.0f, -0.3f);

    std::vector<Vertex> vertices = {
        // Front (+Z)
        {Eigen::Vector3f(-1.0f, -1.0f,  1.0f), Eigen::Vector3f( 0.0f,  0.0f,  1.0f)},
        {Eigen::Vector3f( 1.0f, -1.0f,  1.0f), Eigen::Vector3f( 0.0f,  0.0f,  1.0f)},
        {Eigen::Vector3f( 1.0f,  1.0f,  1.0f), Eigen::Vector3f( 0.0f,  0.0f,  1.0f)},
        {Eigen::Vector3f(-1.0f,  1.0f,  1.0f), Eigen::Vector3f( 0.0f,  0.0f,  1.0f)},

        // Back (-Z)
        {Eigen::Vector3f( 1.0f, -1.0f, -1.0f), Eigen::Vector3f( 0.0f,  0.0f, -1.0f)},
        {Eigen::Vector3f(-1.0f, -1.0f, -1.0f), Eigen::Vector3f( 0.0f,  0.0f, -1.0f)},
        {Eigen::Vector3f(-1.0f,  1.0f, -1.0f), Eigen::Vector3f( 0.0f,  0.0f, -1.0f)},
        {Eigen::Vector3f( 1.0f,  1.0f, -1.0f), Eigen::Vector3f( 0.0f,  0.0f, -1.0f)},

        // Left (-X)
        {Eigen::Vector3f(-1.0f, -1.0f, -1.0f), Eigen::Vector3f(-1.0f,  0.0f,  0.0f)},
        {Eigen::Vector3f(-1.0f, -1.0f,  1.0f), Eigen::Vector3f(-1.0f,  0.0f,  0.0f)},
        {Eigen::Vector3f(-1.0f,  1.0f,  1.0f), Eigen::Vector3f(-1.0f,  0.0f,  0.0f)},
        {Eigen::Vector3f(-1.0f,  1.0f, -1.0f), Eigen::Vector3f(-1.0f,  0.0f,  0.0f)},

        // Right (+X)
        {Eigen::Vector3f( 1.0f, -1.0f,  1.0f), Eigen::Vector3f( 1.0f,  0.0f,  0.0f)},
        {Eigen::Vector3f( 1.0f, -1.0f, -1.0f), Eigen::Vector3f( 1.0f,  0.0f,  0.0f)},
        {Eigen::Vector3f( 1.0f,  1.0f, -1.0f), Eigen::Vector3f( 1.0f,  0.0f,  0.0f)},
        {Eigen::Vector3f( 1.0f,  1.0f,  1.0f), Eigen::Vector3f( 1.0f,  0.0f,  0.0f)},

        // Top (+Y)
        {Eigen::Vector3f(-1.0f,  1.0f,  1.0f), Eigen::Vector3f( 0.0f,  1.0f,  0.0f)},
        {Eigen::Vector3f( 1.0f,  1.0f,  1.0f), Eigen::Vector3f( 0.0f,  1.0f,  0.0f)},
        {Eigen::Vector3f( 1.0f,  1.0f, -1.0f), Eigen::Vector3f( 0.0f,  1.0f,  0.0f)},
        {Eigen::Vector3f(-1.0f,  1.0f, -1.0f), Eigen::Vector3f( 0.0f,  1.0f,  0.0f)},

        // Bottom (-Y)
        {Eigen::Vector3f(-1.0f, -1.0f, -1.0f), Eigen::Vector3f( 0.0f, -1.0f,  0.0f)},
        {Eigen::Vector3f( 1.0f, -1.0f, -1.0f), Eigen::Vector3f( 0.0f, -1.0f,  0.0f)},
        {Eigen::Vector3f( 1.0f, -1.0f,  1.0f), Eigen::Vector3f( 0.0f, -1.0f,  0.0f)},
        {Eigen::Vector3f(-1.0f, -1.0f,  1.0f), Eigen::Vector3f( 0.0f, -1.0f,  0.0f)},
    };

    std::vector<std::uint32_t> indices = {
        0,  1,  2,   2,  3,  0,   // Front
        4,  5,  6,   6,  7,  4,   // Back
        8,  9, 10,  10, 11,  8,   // Left
        12, 13, 14,  14, 15, 12,   // Right
        16, 17, 18,  18, 19, 16,   // Top
        20, 21, 22,  22, 23, 20    // Bottom
    };

    auto cubeMesh = std::make_shared<Mesh>(
        std::move(vertices),
        std::move(indices)
    );

    Scene scene;    
    SceneObject object("Cube", cubeMesh, {1.0f, 0.5f, 0.3f});
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
    renderer.draw(scene, camera, shader, lightDirection);

    auto rgb = target.readRgb();
    auto depth = target.readDepth();
    assert(rgb.size() == 640 * 480 * 3);
    assert(depth.size() == 640 * 480);

    RenderTarget::bindDefault(800, 600);

    while (!glfwWindowShouldClose(window.get())) {        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.draw(scene, camera, shader, lightDirection);

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }


    return 0;
}
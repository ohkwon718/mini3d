#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <numbers>
#include <cstdint>
#include <memory>
#include <cassert>
#include "Platform/GlfwRuntime.hpp"
#include "Platform/FreeCameraController.hpp"
#include "Render/ShaderProgram.hpp"
#include "Render/Renderer.hpp"
#include "Render/RenderTarget.hpp"
#include "Scene/SceneObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/DirectionalLight.hpp"
#include "Camera/Camera.hpp"
#include "Geometry/PrimitiveMeshes.hpp"

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

    // Eigen::Vector3f lightDirection(-0.5f, -1.0f, -0.3f);
    const DirectionalLight& light{
        {-0.5f, -1.0f, -0.3f},
        {1.0f, 0.2f, 0.2f},
        0.2f
    };
    
    auto cubeMesh = std::make_shared<const Mesh>(
        createCubeMesh()
    );
    auto sphereMesh = std::make_shared<const Mesh>(
        createUvSphereMesh(1.0f, 32, 64)
    );

    Scene scene;    
    SceneObject CubeCenter("Cube", cubeMesh, {1.0f, 0.3f, 0.3f});
    CubeCenter.transform().setTranslation({0.0f, 0.0f, -10.0f});    
    CubeCenter.transform().setRotation({0.5f, 0.5f, 0.5f, 1.0f});
    
    SceneObject CubeLeft("Cube", cubeMesh, {0.3f, 1.0f, 0.3f});
    CubeLeft.transform().setTranslation({-3.0f, 0.0f, -10.0f});    
    CubeLeft.transform().setRotation({0.5f, 0.5f, 0.1f, 1.0f});    

    SceneObject CubeRight("Cube", cubeMesh, {0.3f, 0.3f, 1.0f});
    CubeRight.transform().setTranslation({3.0f, 0.0f, -10.0f});    
    CubeRight.transform().setRotation({0.5f, 0.5f, 0.9f, 1.0f});
    
    scene.addObject(std::move(CubeCenter));
    scene.addObject(std::move(CubeLeft));
    scene.addObject(std::move(CubeRight));

    SceneObject sphereLeft("Sphere", sphereMesh, {1.0f, 0.3f, 0.3f, 8.0f, 0.1f});
    sphereLeft.transform().setTranslation({-3.0f, 3.0f, -10.0f});
    scene.addObject(std::move(sphereLeft));
    

    SceneObject sphereRight("Sphere", sphereMesh, {0.8f, 0.8f, 0.8f, 128.0f, 1.0f});
    sphereRight.transform().setTranslation({3.0f, 3.0f, -10.0f});
    scene.addObject(std::move(sphereRight));


    Renderer renderer;

    ShaderProgram shader = 
            ShaderProgram::fromFiles(   "assets/shaders/lit.vert",
                                        "assets/shaders/lit.frag");
    
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
    renderer.draw(scene, camera, shader, light);

    auto rgb = target.readRgb();
    auto depth = target.readDepth();
    assert(rgb.size() == 640 * 480 * 3);
    assert(depth.size() == 640 * 480);

    RenderTarget::bindDefault(800, 600);
    FreeCameraController controller(5.0f, 0.002f);

    double previousTime = glfwGetTime();
    while (!glfwWindowShouldClose(window.get())) {        
        glfwPollEvents();

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        previousTime = currentTime;
        controller.update(
            window.get(),
            camera,
            static_cast<float>(deltaTime)
        );

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.draw(scene, camera, shader, light);

        glfwSwapBuffers(window.get());
    }


    return 0;
}
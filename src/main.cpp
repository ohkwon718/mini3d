#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Platform/GlfwRuntime.hpp"
#include "Platform/FreeCameraController.hpp"
#include "Render/ShaderProgram.hpp"
#include "Render/Renderer.hpp"
#include "Render/RenderTarget.hpp"
#include "Render/Viewport.hpp"
#include "Scene/SceneObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/DirectionalLight.hpp"
#include "Scene/Material.hpp"
#include "Camera/Camera.hpp"
#include "Camera/CameraIntrinsics.hpp"
#include "Geometry/PrimitiveMeshes.hpp"
#include "core/Image.hpp"
#include "core/ImageLoader.hpp"
#include "Assets/GltfLoader.hpp"
#include "Simulation/Scenario.hpp"
#include "Transform/Pose.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    ///////////////////////////////////////////

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ///////////////////////////////////////////
    const DirectionalLight light{
        {-0.5f, 1.0f, -0.3f},
        {1.0f, 1.0f, 1.0f},
        0.5f
    };

    // ///////////////////////////////////////////

    Renderer renderer;

    ShaderProgram shader = 
            ShaderProgram::fromFiles(   "assets/shaders/lit.vert",
                                        "assets/shaders/lit.frag");
    
    // ///////////////////////////////////////////

    const Scenario scenario = loadScenario("assets/scenarios/classroom.json");
    Scene scene = loadGltfScene(scenario.worldPath());    
        
    // const SensorRig& rig = scenario.rig("robot");

    const Trajectory& trajectory = scenario.trajectory("robot_path");
    SensorRig rig = scenario.rig(trajectory.rigName());    
    Camera sensorCamera = rig.camera("front_camera");
    RenderTarget sensorTarget(
        sensorCamera.intrinsics().width,
        sensorCamera.intrinsics().height
    );

    Camera viewerCamera = rig.camera("front_camera");  // temp
    FreeCameraController controller(5.0f, 0.002f);

    double simulationTime = 0.0;
    double previousTime = glfwGetTime();
    while (!glfwWindowShouldClose(window.get())) {        

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("mini3d");
        ImGui::Text("ImGui works");
        ImGui::End();

        if (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window.get(), GLFW_TRUE);
        }

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        previousTime = currentTime;
        controller.update(
            window.get(),
            viewerCamera,
            static_cast<float>(deltaTime)
        );

        simulationTime += deltaTime;
        rig.setPose(trajectory.sample(simulationTime));
        Camera sensorCamera = rig.camera("front_camera");        

        sensorTarget.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.draw(scene, sensorCamera, shader, light);
        
        int framebufferWidth;
        int framebufferHeight;
        
        glfwGetFramebufferSize(
            window.get(),
            &framebufferWidth,
            &framebufferHeight
        );

        if (framebufferWidth > 0 && framebufferHeight > 0) {
            const auto& intrinsics = viewerCamera.intrinsics();
            const Viewport viewport = fitViewport(
                framebufferWidth,
                framebufferHeight,
                intrinsics.width,
                intrinsics.height
            );            
            RenderTarget::bindDefault(viewport);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderer.draw(scene, viewerCamera, shader, light);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(
                ImGui::GetDrawData()
            );

            glfwSwapBuffers(window.get());
        }

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}
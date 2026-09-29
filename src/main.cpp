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
#include "CV/PointCloud.hpp"
#include "core/RgbImage.hpp"
#include "CV/Triangulation.hpp"
#include "CV/OpenCvInterop.hpp"
#include "Assets/GltfLoader.hpp"
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>



#include <algorithm>
#include <fstream>


namespace {

void saveDepthPreview(
    const DepthImage& depth,
    const Camera& camera,
    const std::string& path)
{
    constexpr float maxVisualDepth = 10.0f;

    const std::size_t width =
        static_cast<std::size_t>(depth.width());

    const std::size_t height =
        static_cast<std::size_t>(depth.height());

    std::vector<std::uint8_t> pixels(width * height);

    for (std::size_t y = 0; y < height; ++y) {

        const auto depthRow = depth.row(y);

        for (std::size_t x = 0; x < width; ++x) {

            const float raw = depthRow[x];
            const std::size_t index = y * width + x;

            if (raw >= 1.0f) {
                pixels[index] = 0;
                continue;
            }

            const float metricDepth =
                camera.depthToMetric(raw);

            const float normalized =
                1.0f - std::clamp(
                    metricDepth / maxVisualDepth,
                    0.0f,
                    1.0f
                );

            pixels[index] =
                static_cast<std::uint8_t>(
                    normalized * 255.0f
                );
        }
    }


    std::ofstream file(path, std::ios::binary);

    file << "P5\n"
         << width << ' ' << height << "\n255\n";

    file.write(
        reinterpret_cast<const char*>(pixels.data()),
        static_cast<std::streamsize>(pixels.size())
    );
}

}


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

    const DirectionalLight light{
        {-0.5f, 1.0f, -0.3f},
        {1.0f, 1.0f, 1.0f},
        0.5f
    };
    
    Scene scene;    

    auto cubeMesh = std::make_shared<const Mesh>(
        createCubeMesh()
    );

    SceneObject CubeCenter("Cube", cubeMesh, {1.0f, 0.3f, 0.3f});
    CubeCenter.transform().setTranslation({0.0f, 0.0f, -10.0f});    
    CubeCenter.transform().setRotation({0.5f, 0.5f, 0.5f, 1.0f});
    scene.addObject(std::move(CubeCenter));    
    
    SceneObject CubeLeft("Cube", cubeMesh, {0.3f, 1.0f, 0.3f});
    CubeLeft.transform().setTranslation({-3.0f, 0.0f, -10.0f});    
    CubeLeft.transform().setRotation({0.5f, 0.5f, 0.1f, 1.0f});    
    scene.addObject(std::move(CubeLeft));

    SceneObject CubeRight("Cube", cubeMesh, {0.3f, 0.3f, 1.0f});
    CubeRight.transform().setTranslation({3.0f, 0.0f, -10.0f});    
    CubeRight.transform().setRotation({0.5f, 0.5f, 0.9f, 1.0f});
    scene.addObject(std::move(CubeRight));

    
    auto sphereMesh = std::make_shared<const Mesh>(
        createUvSphereMesh(1.0f, 32, 64)
    );    

    SceneObject sphereLeft("Sphere", sphereMesh, {1.0f, 0.3f, 0.3f, 8.0f, 0.1f});
    sphereLeft.transform().setTranslation({-3.0f, 3.0f, -10.0f});
    scene.addObject(std::move(sphereLeft));    

    SceneObject sphereRight("Sphere", sphereMesh, {0.8f, 0.8f, 0.8f, 128.0f, 1.0f});
    sphereRight.transform().setTranslation({3.0f, 3.0f, -10.0f});
    scene.addObject(std::move(sphereRight));


    auto planeMesh = std::make_shared<const Mesh>(
        createPlaneMesh(15.0f)
    );

    auto groundImage = std::make_shared<const Image>(
        loadImage("assets/textures/ground.png")
    );         
    SceneObject ground(
        "Ground",
        planeMesh,
        {0.35f, 0.35f, 0.35f, 16.0f, 0.1f, groundImage}        
    );
    ground.transform().setTranslation({0.0f, -1.5f, -10.0f});
    scene.addObject(std::move(ground));        

    
    auto loadedObjects = loadGltfObjects(
    //     "assets/models/DamagedHelmet.glb"
        "assets/models/CesiumMilkTruck.glb"
    );
    for (auto& loaded : loadedObjects) {
        scene.addObject(
            SceneObject(
                std::move(loaded.name),
                std::move(loaded.mesh),
                std::move(loaded.material),
                std::move(loaded.transform)
            )
        );
    }

    ///////////////////////////////////////////


    Renderer renderer;

    ShaderProgram shader = 
            ShaderProgram::fromFiles(   "assets/shaders/lit.vert",
                                        "assets/shaders/lit.frag");
    
                                        
    ///////////////////////////////////////////

    CameraIntrinsics intrinsics{
        800,
        600,
        724.264f,
        724.264f,
        400.0f,
        300.0f
    };

    Camera camera(
        intrinsics,
        0.1f,
        100.0f
    );
    camera.setPosition({0.0f, 0.0f, 3.0f});


    Camera cameraA(
        intrinsics,
        0.1f,
        100.0f
    );
    cameraA.setPosition({-0.5f, 0.0f, 3.0f});

    Camera cameraB(
        intrinsics,
        0.1f,
        100.0f
    );
    cameraB.setPosition({ 0.5f, 0.0f, 3.0f});
    
    Camera cameraC(
        intrinsics,
        0.1f,
        100.0f
    );
    cameraC.setPosition({ 0.0f, 0.5f, 3.0f});
    const Eigen::Vector3f worldPoint(1.5f, 2.3f, -4.0f);



    Eigen::Vector2f pixelA = cameraA.project(worldPoint);
    Eigen::Vector2f pixelB = cameraB.project(worldPoint);
    Eigen::Vector2f pixelC = cameraC.project(worldPoint);    
    
    const Eigen::Vector2f noisyA = pixelA + Eigen::Vector2f(0.4f, -0.2f);
    const Eigen::Vector2f noisyB = pixelB + Eigen::Vector2f(-0.3f, 0.5f);
    
    const std::array<CameraObservation, 3> observations{{
        {&cameraA, noisyA},
        {&cameraB, noisyB},
        {&cameraC, pixelC}
    }};
    const Eigen::Vector3f linearSvd = triangulateLinearSvd(observations);    

    std::cout << worldPoint << std::endl;
    std::cout << linearSvd << std::endl;
    // std::cout << closestRays << std::endl;
    
    
    // Eigen::Vector3f linearSvd = triangulateLinearSvd(cameraA, noisyA, cameraB, noisyB);
    // Eigen::Vector3f closestRays = triangulateClosestRays(cameraA, noisyA, cameraB, noisyB);
    

    ///////////////////////////////////////////
    
    RenderTarget target(
        camera.intrinsics().width,
        camera.intrinsics().height
    );
    target.bind();    

    {
        target.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.draw(scene, cameraA, shader, light);

        RgbImage rgbA = target.readRgb();
        cv::Mat imageA = toCvMatCopy(rgbA);
        
        target.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.draw(scene, cameraB, shader, light);

        RgbImage rgbB = target.readRgb();
        cv::Mat imageB = toCvMatCopy(rgbB);


    }


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer.draw(scene, camera, shader, light);

    RgbImage rgb = target.readRgb();
    DepthImage depth = target.readDepth();
    
    cv::Mat mat = toCvMatCopy(rgb);    
    std::cout << mat.size() << std::endl;    
    cv::imwrite("./cvmat.png", mat);
    


    auto pcd = PointCloud::fromRgbd(
        depth,
        rgb,
        camera
    );     
    pcd.savePointCloudPly("pointcloud.ply");

    saveDepthPreview(
        depth,
        camera,
        "depth_preview.pgm"
    );
    
    FreeCameraController controller(5.0f, 0.002f);

    double previousTime = glfwGetTime();
    while (!glfwWindowShouldClose(window.get())) {        
        glfwPollEvents();
        if (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window.get(), GLFW_TRUE);
        }
        int framebufferWidth;
        int framebufferHeight;
        
        glfwGetFramebufferSize(
            window.get(),
            &framebufferWidth,
            &framebufferHeight
        );

        if (framebufferWidth > 0 && framebufferHeight > 0) {            

            const auto& intrinsics = camera.intrinsics();
            const Viewport viewport = fitViewport(
                framebufferWidth,
                framebufferHeight,
                intrinsics.width,
                intrinsics.height
            );            
            RenderTarget::bindDefault(viewport);
        }

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
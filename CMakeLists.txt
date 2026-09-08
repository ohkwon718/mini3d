cmake_minimum_required(VERSION 3.28)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    mini3d
    VERSION 0.1.0
    LANGUAGES C CXX
)

find_package (Eigen3 REQUIRED NO_MODULE)
find_package (OpenGL REQUIRED)
find_package (glfw3 REQUIRED)

enable_testing()
add_test(
    NAME transform_test
    COMMAND transform_test
)
add_test(
    NAME camera_test
    COMMAND camera_test
)
add_test(
    NAME scene_test
    COMMAND scene_test
)

include(FetchContent)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
    SOURCE_SUBDIR cmake
)

FetchContent_MakeAvailable(glad)

glad_add_library(
    glad_gl_core_33
    STATIC
    REPRODUCIBLE
    API gl:core=3.3
)


add_library(
    mini3d_core
    src/Transform/Transform.cpp
    src/Scene/SceneObject.cpp
    src/Scene/Scene.cpp
    src/Camera/Camera.cpp
    src/Geometry/Mesh.cpp
    src/Geometry/PrimitiveMeshes.cpp
)
target_compile_features(mini3d_core PRIVATE cxx_std_20)
target_include_directories(
    mini3d_core 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(
    mini3d_core
    PUBLIC Eigen3::Eigen
)
target_compile_options(
    mini3d_core
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
)

add_library(
    mini3d_render
    src/Render/VertexBuffer.cpp
    src/Render/VertexArray.cpp
    src/Render/IndexBuffer.cpp
    src/Render/ShaderProgram.cpp
    src/Render/GpuMesh.cpp
    src/Render/Renderer.cpp
    src/Render/RenderTarget.cpp
)
target_compile_features(mini3d_render PRIVATE cxx_std_20)
target_include_directories(
    mini3d_render 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(
    mini3d_render
    PUBLIC
        mini3d_core
        Eigen3::Eigen
        glad_gl_core_33
    PRIVATE
        OpenGL::GL
)
target_compile_options(
    mini3d_render
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
)

add_executable(
    mini3d
    src/main.cpp
    src/Platform/GlfwRuntime.cpp 
    src/Platform/FreeCameraController.cpp
)
target_compile_features(mini3d PRIVATE cxx_std_20)
target_link_libraries(
    mini3d    
    PRIVATE 
        mini3d_render
        glfw
        OpenGL::GL
        glad_gl_core_33
)
target_compile_options(
    mini3d
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
)

add_executable(
    transform_test
    tests/TransformTest.cpp    
)
target_compile_features(transform_test PRIVATE cxx_std_20)
target_link_libraries(
    transform_test    
    PRIVATE mini3d_core
)
target_compile_options(       
    transform_test 
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
)

add_executable(
    camera_test
    tests/CameraTest.cpp    
)
target_compile_features(camera_test PRIVATE cxx_std_20)
target_link_libraries(
    camera_test    
    PRIVATE mini3d_core
)
target_compile_options(       
    camera_test 
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
)


add_executable(
    scene_test
    tests/SceneTest.cpp    
)
target_compile_features(scene_test PRIVATE cxx_std_20)
target_link_libraries(
    scene_test    
    PRIVATE mini3d_core
)
target_compile_options(       
    scene_test 
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
)

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "${CMAKE_BINARY_DIR}/assets"
)

add_dependencies(mini3d copy_assets)
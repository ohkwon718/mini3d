cmake_minimum_required(VERSION 3.28)

project(
    mini3d
    VERSION 0.1.0
    LANGUAGES CXX
)

find_package (Eigen3 REQUIRED NO_MODULE)

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


add_library(
    mini3d_core
    src/Transform/Transform.cpp
    src/Scene/SceneObject.cpp
    src/Camera/Camera.cpp
    src/Geometry/Mesh.cpp
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

add_executable(
    mini3d
    src/main.cpp
)
target_compile_features(mini3d PRIVATE cxx_std_20)
target_link_libraries(
    mini3d    
    PRIVATE mini3d_core
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
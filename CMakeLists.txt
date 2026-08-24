cmake_minimum_required(VERSION 3.28)

project(
    mini3d
    VERSION 0.1.0
    LANGUAGES CXX
)

find_package (Eigen3 REQUIRED NO_MODULE)

add_library(
    mini3d_core
    src/Transform/Transform.cpp
)

target_compile_features(
    mini3d_core    
    PUBLIC cxx_std_20
)

target_include_directories(
    mini3d_core 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
    mini3d_core
    PUBLIC Eigen3::Eigen
)

add_executable(
    mini3d
    src/main.cpp
)

target_link_libraries(
    mini3d    
    PRIVATE mini3d_core
)

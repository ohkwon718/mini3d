#pragma once

#include <memory>
#include <unordered_map>
#include "Scene/SceneObject.hpp"
#include "Scene/Scene.hpp"
#include "Camera/Camera.hpp"
#include "ShaderProgram.hpp"
#include "GpuMesh.hpp"


class Renderer
{
public:
    void draw(
        const SceneObject& object,
        const Camera& camera,
        const ShaderProgram& shader,
        const Eigen::Vector3f& lightDirection        
    );

    void draw(
        const Scene& scene,
        const Camera& camera,
        const ShaderProgram& shader,
        const Eigen::Vector3f& lightDirection
    );

private:
    std::unordered_map<
        std::shared_ptr<const Mesh>,
        GpuMesh
    > gpuMeshes_;
};
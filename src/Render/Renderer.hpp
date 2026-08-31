#pragma once

#include "Scene/SceneObject.hpp"
#include "Camera/Camera.hpp"
#include "ShaderProgram.hpp"
#include "GpuMesh.hpp"


class Renderer
{
public:
    void draw(const SceneObject& object,
              const Camera& camera,
              ShaderProgram& shader);

private:
    std::unordered_map<
        std::shared_ptr<const Mesh>,
        GpuMesh
    > gpuMeshes_;
};
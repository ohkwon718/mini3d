#pragma once

#include <memory>
#include <unordered_map>
#include "Scene/SceneObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/DirectionalLight.hpp"
#include "Camera/Camera.hpp"
#include "ShaderProgram.hpp"
#include "GpuMesh.hpp"
#include "Render/Texture2D.hpp"

class Renderer
{
public:    
    void draw(
        const Scene& scene,
        const Camera& camera,
        const ShaderProgram& shader,
        const DirectionalLight& lightDirection
    );

private:
    void drawObject(
        const SceneObject& object,
        const ShaderProgram& shader        
    );

    const Texture2D& textureFor(
        const std::shared_ptr<const Image>& image
    );

    std::unordered_map<
        std::shared_ptr<const Mesh>,
        GpuMesh
    > gpuMeshes_;

    std::unordered_map<
        std::shared_ptr<const Image>,
        Texture2D
    > textureCache_;
};
#include "Renderer.hpp"

void Renderer::draw(const SceneObject& object,
                    const Camera& camera,
                    const ShaderProgram& shader,
                    const Eigen::Vector3f& lightDirection,
                    const Eigen::Vector3f& baseColor)
{   
    const auto& mesh = object.mesh();

    auto it = gpuMeshes_.try_emplace(mesh, *mesh).first;

    shader.use();
    shader.setMat4("uModel", object.transform().matrix());
    shader.setMat4("uView", camera.viewMatrix());
    shader.setMat4("uProjection", camera.projectionMatrix());
    shader.setVec3("uLightDirection", lightDirection);
    shader.setVec3("uBaseColor", baseColor);

    it->second.draw();
}


void Renderer::draw(const Scene& scene,
                    const Camera& camera,
                    const ShaderProgram& shader,
                    const Eigen::Vector3f& lightDirection,
                    const Eigen::Vector3f& baseColor)
{   
    for (std::size_t i = 0; i < scene.size(); ++i) {
        draw(scene.object(i), camera, shader, lightDirection, baseColor);
    }
}


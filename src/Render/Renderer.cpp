#include "Renderer.hpp"

void Renderer::draw(const SceneObject& object,
                    const Camera& camera,
                    ShaderProgram& shader)
{   
    const auto& mesh = object.mesh();

    auto [it, inserted] =
        gpuMeshes_.try_emplace(mesh, *mesh);

    shader.use();
    shader.setMat4("uModel", object.transform().matrix());
    shader.setMat4("uView", camera.viewMatrix());
    shader.setMat4("uProjection", camera.projectionMatrix());

    it->second.draw();
}


void Renderer::draw(const Scene& scene,
                    const Camera& camera,
                    ShaderProgram& shader)
{   
    for (std::size_t i = 0; i < scene.size(); ++i) {
        draw(scene.object(i), camera, shader);
    }
}


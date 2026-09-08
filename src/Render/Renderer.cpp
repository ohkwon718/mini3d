#include "Renderer.hpp"

void Renderer::draw(const SceneObject& object,
                    const Camera& camera,
                    const ShaderProgram& shader,
                    const DirectionalLight& light)
{   
    const auto& mesh = object.mesh();

    auto it = gpuMeshes_.try_emplace(mesh, *mesh).first;

    shader.use();
    shader.setMat4("uModel", object.transform().matrix());
    shader.setMat4("uView", camera.viewMatrix());
    shader.setMat4("uProjection", camera.projectionMatrix());
    shader.setVec3("uLightDirection", light.direction);
    shader.setVec3("uLightColor", light.color);
    shader.setFloat("uLightIntensity", light.intensity);

    shader.setVec3("uBaseColor", object.material().baseColor);
    shader.setVec3("uCameraPosition", camera.position());
    shader.setFloat("uShininess", object.material().shininess);
    shader.setFloat("uSpecularStrength", object.material().specularStrength);
   

    it->second.draw();
}


void Renderer::draw(const Scene& scene,
                    const Camera& camera,
                    const ShaderProgram& shader,
                    const DirectionalLight& light)
{   
    for (std::size_t i = 0; i < scene.size(); ++i) {
        draw(scene.object(i), camera, shader, light);
    }
}


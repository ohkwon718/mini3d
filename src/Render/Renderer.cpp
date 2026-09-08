#include "Renderer.hpp"


void Renderer::draw(const Scene& scene,
                    const Camera& camera,
                    const ShaderProgram& shader,
                    const DirectionalLight& light)
{   
    shader.use();    
    shader.setMat4("uView", camera.viewMatrix());
    shader.setMat4("uProjection", camera.projectionMatrix());
    shader.setVec3("uCameraPosition", camera.position());
    shader.setVec3("uLightDirection", light.direction);
    shader.setVec3("uLightColor", light.color);
    shader.setFloat("uLightIntensity", light.intensity);

    for (std::size_t i = 0; i < scene.size(); ++i) {
        drawObject(scene.object(i), shader);
    }
}


void Renderer::drawObject(const SceneObject& object,
                    const ShaderProgram& shader)
{   
    const auto& mesh = object.mesh();

    auto it = gpuMeshes_.try_emplace(mesh, *mesh).first;

    shader.setMat4("uModel", object.transform().matrix());
    shader.setVec3("uBaseColor", object.material().baseColor);    
    shader.setFloat("uShininess", object.material().shininess);
    shader.setFloat("uSpecularStrength", object.material().specularStrength);   

    it->second.draw();
}


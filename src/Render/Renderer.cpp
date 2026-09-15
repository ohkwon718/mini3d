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
    shader.setInt("uTexture", 0);

    for (std::size_t i = 0; i < scene.size(); ++i) {
        drawObject(scene.object(i), shader);
    }    
}


void Renderer::drawObject(
    const SceneObject& object,
    const ShaderProgram& shader)
{   
    const auto& mesh = object.mesh();
    auto it = gpuMeshes_.try_emplace(mesh, *mesh).first;
    
    const auto& material = object.material();
    const Texture2D& texture = textureFor(material.baseColorImage);    

    texture.bind();

    shader.setMat4("uModel", object.transform().matrix());
    shader.setVec3("uBaseColor", material.baseColor);    
    shader.setFloat("uShininess", material.shininess);
    shader.setFloat("uSpecularStrength", material.specularStrength);   

    it->second.draw();
}

const Texture2D& Renderer::textureFor(
    const std::shared_ptr<const Image>& image)
{
    if (image) {
        auto it = textureCache_.try_emplace(image, *image).first;
        return it->second;
    }
         
    auto it = textureCache_.try_emplace(image).first;
    return it->second;
    
}
#include "SceneObject.hpp"
#include <utility>
#include <stdexcept>

SceneObject::SceneObject(
        std::string name, 
        std::shared_ptr<const Mesh> mesh, 
        Material material,
        Transform transform
    )
    : name_(std::move(name)),
      transform_{std::move(transform)},
      mesh_(std::move(mesh)),
      material_{std::move(material)}      
{
    if (!mesh_) {
        throw std::invalid_argument("SceneObject requires a mesh");
    }
}


const std::string& SceneObject::name() const
{
    return name_;
}

Transform& SceneObject::transform()
{
    return transform_;
}

const Transform& SceneObject::transform() const
{
    return transform_;
}

const std::shared_ptr<const Mesh>& SceneObject::mesh() const
{
    return mesh_;
}

const Material& SceneObject::material() const
{
    return material_;
}
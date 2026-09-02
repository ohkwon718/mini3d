#include "SceneObject.hpp"
#include <utility>
#include <stdexcept>

SceneObject::SceneObject(std::string name, std::shared_ptr<const Mesh> mesh)
    : name_(std::move(name)),
      mesh_(std::move(mesh))
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
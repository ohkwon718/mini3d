#include "SceneObject.hpp"
#include <utility>

SceneObject::SceneObject(std::string name)
    : name_(std::move(name))
{
}

SceneObject::SceneObject(std::string name, std::shared_ptr<const Mesh> mesh)
    : name_(std::move(name)),
      mesh_(std::move(mesh))
{
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

std::shared_ptr<const Mesh> SceneObject::mesh() const
{
    return mesh_;
}
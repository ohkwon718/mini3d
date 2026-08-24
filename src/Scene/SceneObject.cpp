#include "SceneObject.hpp"
#include <utility>

SceneObject::SceneObject(std::string name)
    : name_(std::move(name))
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

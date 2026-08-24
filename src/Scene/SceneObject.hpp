#pragma once

#include <Transform/Transform.hpp>

class SceneObject {
public:    
    SceneObject(std::string name);    
    const std::string& name() const;

    Transform& transform();
    const Transform& transform() const;

private:
    std::string name_;
    Transform transform_;

};

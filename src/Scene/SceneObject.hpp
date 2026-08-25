#pragma once

#include <memory>
#include "Transform/Transform.hpp"
#include "Geometry/Mesh.hpp"

class SceneObject {
public:    
    SceneObject(std::string name);    
    SceneObject(std::string name, std::shared_ptr<const Mesh> mesh);

    const std::string& name() const;

    Transform& transform();
    const Transform& transform() const;

    std::shared_ptr<const Mesh> mesh() const;

private:
    std::string name_;
    Transform transform_;
    
    std::shared_ptr<const Mesh> mesh_;

};

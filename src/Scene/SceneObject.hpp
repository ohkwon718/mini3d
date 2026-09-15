#pragma once

#include <memory>
#include <string>
#include "Transform/Transform.hpp"
#include "Geometry/Mesh.hpp"
#include "Material.hpp"


class SceneObject {
public:    
    SceneObject(
        std::string name, 
        std::shared_ptr<const Mesh> mesh, 
        Material material = {},
        Transform transform = {}
    );

    const std::string& name() const;

    Transform& transform();
    const Transform& transform() const;
        
    const std::shared_ptr<const Mesh>& mesh() const;
    const Material& material() const;

private:
    std::string name_;
    Transform transform_;    
    std::shared_ptr<const Mesh> mesh_;
    Material material_;

};

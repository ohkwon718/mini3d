#pragma once

#include <vector>
#include <cstddef>
#include "SceneObject.hpp"

class Scene {
public:
    void addObject(SceneObject object);

    std::size_t size() const;

    SceneObject& object(std::size_t index);
    const SceneObject& object(std::size_t index) const;

private:
    std::vector<SceneObject> objects_;
};

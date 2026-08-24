#include "Scene.hpp"
#include <utility>

void Scene::addObject(SceneObject object) {
    objects_.push_back(std::move(object));
}

std::size_t Scene::size() const {
    return objects_.size();
}

SceneObject& Scene::object(std::size_t index) {
    return objects_[index];
}

const SceneObject& Scene::object(std::size_t index) const {
    return objects_[index];
}

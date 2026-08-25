#include <iostream>
#include "Transform/Transform.hpp"
#include "Scene/SceneObject.hpp"
#include "Geometry/Mesh.hpp"

int main() {

    auto mesh = std::make_shared<Mesh>(
        std::vector<Vertex>{
            {Eigen::Vector3f(0.0f, 0.0f, 0.0f)},
            {Eigen::Vector3f(1.0f, 0.0f, 0.0f)},
            {Eigen::Vector3f(0.0f, 1.0f, 0.0f)}
        },
        std::vector<std::uint32_t>{0, 1, 2}
    );

    SceneObject obj1("Triangle1", mesh);
    SceneObject obj2("Triangle2", mesh);


    return 0;
}
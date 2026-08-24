#include <iostream>
#include "Transform/Transform.hpp"
#include "Scene/SceneObject.hpp"

int main() {
    SceneObject obj("MyObject");
    std::cout << "SceneObject name: " << obj.name() << std::endl;


    return 0;
}
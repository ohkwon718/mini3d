#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

#include "Scene/SceneObject.hpp"
#include "Geometry/Mesh.hpp"

int main() {

    auto mesh = std::make_shared<const Mesh>(
        std::vector<Vertex>{
            {Eigen::Vector3f(0.0f, 0.0f, 0.0f)},
            {Eigen::Vector3f(1.0f, 0.0f, 0.0f)},
            {Eigen::Vector3f(0.0f, 1.0f, 0.0f)}
        },
        std::vector<std::uint32_t>{0, 1, 2}
    );

    SceneObject a("A", mesh);
    SceneObject b("B", mesh);

    assert(a.mesh() == b.mesh());

    a.transform().setTranslation(Eigen::Vector3f(1.0f, 2.0f, 3.0f));
    assert(
        b.transform().translation().isApprox(Eigen::Vector3f::Zero())
    );

    return 0;
}
#ifndef SCENE_HPP
#define SCENE_HPP

#include <ebb/core/geometry/mesh.hpp>
#include <ebb/core/shaders/light.hpp>
#include <vector>

namespace Ebb {
namespace Core {

class Scene {
public:
    Scene();
    Scene(
        std::vector<Ebb::Core::Mesh *>& meshes,
        std::vector<Ebb::Core::LightSource *>& lights);

    std::vector<Ebb::Core::LightSource *> get_lights() {
        return this->lights;
    }
    int num_lights() {
        return this->lights.size();
    }
private:
    std::vector<Ebb::Core::Mesh *> meshes;
    std::vector<Ebb::Core::LightSource *> lights;
};

}; };

#endif // SCENE_HPP
#ifndef SHADERS_BASICS_HPP
#define SHADERS_BASICS_HPP

#include <ebb/core/shaders/shader.hpp>
#include <ebb/math/vector.hpp>

namespace Ebb {
namespace Core {

class BlinnPhongShader : public Ebb::Core::ShaderProgram {
public:
    float3  diffuseColor;
    float3 specularColor;

private:
};

}; };

#endif // SHADERS_BASICS_HPP
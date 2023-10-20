#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <ebb/math/math.hpp>

using namespace Ebb::Math;

namespace Ebb {
namespace Core {

class LightSource {
public:
    float3 position;
    float3 color;
    float intensity;

    LightSource(float3 position, float3 color, float intensity);
};

}; };

#endif // LIGHT_HPP
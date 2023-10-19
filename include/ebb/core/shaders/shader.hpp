#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <ebb/math/math.hpp>

using namespace Ebb::Math;

namespace Ebb {
namespace Core {

namespace ShaderCompiler {

unsigned int   compile_vertex(const char *source);
unsigned int compile_fragment(const char *source);

};

class ShaderProgram {
public:
    unsigned int sProgram;
    
    ShaderProgram();
    ~ShaderProgram();

    void load_vertex_source(const char *source);
    void load_fragment_source(const char *source);
    void link();

    void set_bool(const char *name, bool value);
    void set_int(const char *name, int value);
    void set_float(const char *name, float value);

    void set_floats(const char *name, float v0);
    void set_floats(const char *name, float v0, float v1);
    void set_floats(const char *name, float v0, float v1, float v2);
    void set_floats(const char *name, float v0, float v1, float v2, float v3);

    void set_float2(const char *name, float2 value);
    void set_float3(const char *name, float3 value);
    void set_float4(const char *name, float4 value);

    void use();

private:
    unsigned int vShader;
    unsigned int fShader;
};

}; };

#endif // SHADER_HPP
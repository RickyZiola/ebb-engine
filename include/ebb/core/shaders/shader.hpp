#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

namespace Ebb {
namespace Core {

namespace ShaderCompiler {

unsigned int   compile_vertex(const char *source);
unsigned int compile_fragment(const char *source);

};

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    void load_vertex_source(const char *source);
    void load_fragment_source(const char *source);

private:
    unsigned int vShader;
    unsigned int fShader;
    unsigned int sProgram;
};

}; };

#endif // SHADER_HPP
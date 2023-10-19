#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <ebb/core/shaders/shader.hpp>

namespace Ebb {
namespace Core {

class TextureShader : public Ebb::Core::ShaderProgram {
public:
    TextureShader(const char *filename);

    void use();
private:
    unsigned char *data;
    unsigned int textureId;
    int width, height, channels;
};

}; };

#endif
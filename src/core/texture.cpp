#include <ebb/core/shaders/texture.hpp>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <ebb/external/stb_image.h>

static const char * const texture_vert_source = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 texCoord;

uniform mat4x4 projectionMatrix;
uniform mat4x4 viewMatrix;
uniform mat4x4 objectMatrix;

out vec3 normal;
out vec2 uv;
void main() {
    gl_Position = projectionMatrix * (viewMatrix * objectMatrix) * vec4(aPos, 1.0);
    normal = aNorm;
    uv = texCoord;
}
)";

static const char * const texture_frag_source = R"(
#version 330 core
out vec4 FragColor;

uniform sampler2D textureSampler;
in vec2 uv;
in vec3 normal;

void main() {
    FragColor = vec4(texture(textureSampler, uv).rgb, 1.0);
}
)";

Ebb::Core::TextureShader::TextureShader(const char *filename) {
    this->data = stbi_load(filename, &this->width, &this->height, &this->channels, 0);
    if (!data) {
        fprintf(stderr, "Failed to load texture from %s\n", filename);
        return;
    }

    glGenTextures(1, &this->textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set texture wrapping and filtering options (GL_REPEAT for wrapping and GL_LINEAR for filtering)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (this->channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    if (this->channels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    this->load_vertex_source(texture_vert_source);
    this->load_fragment_source(texture_frag_source);
    this->link();
}

void Ebb::Core::TextureShader::use() {
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUseProgram(this->sProgram);
}
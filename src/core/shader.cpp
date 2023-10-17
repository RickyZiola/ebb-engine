#include <ebb/core/shaders/shader.hpp>
#include <stdio.h>

unsigned int Ebb::Core::ShaderCompiler::compile_vertex(const char *source) {
    unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &source, NULL);
    glCompileShader(vert);

    int success;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vert, 512, NULL, infoLog);
        fprintf(stderr, "Vertex shader compilation failed with log: \n%s\n\n", infoLog);
        return -1;
    }

    return vert;
}

unsigned int Ebb::Core::ShaderCompiler::compile_fragment(const char *source) {
    unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &source, NULL);
    glCompileShader(frag);

    int success;
    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(frag, 512, NULL, infoLog);
        fprintf(stderr, "Fragment shader compilation failed with log: \n%s\n\n", infoLog);
        return -1;
    }

    return frag;
}

Ebb::Core::ShaderProgram::ShaderProgram() {
    this->fShader = -1;
    this->vShader = -1;
    this->sProgram= -1;
}

Ebb::Core::ShaderProgram::~ShaderProgram() {
    glDeleteShader(this->fShader);
    glDeleteShader(this->vShader);
    glDeleteProgram(this->sProgram);
}
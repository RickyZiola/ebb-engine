#include <ebb/core/shaders/shader.hpp>
#include <stdio.h>

/**
 * TODO: Preproccessing, custom shading language (?)
*/
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
    }

    return frag;
}

Ebb::Core::ShaderProgram::ShaderProgram() {
    this->fShader = -1;
    this->vShader = -1;
    this->sProgram= glCreateProgram();
}

Ebb::Core::ShaderProgram::~ShaderProgram() {
    glDeleteShader(this->fShader);
    glDeleteShader(this->vShader);
    glDeleteProgram(this->sProgram);
}

void Ebb::Core::ShaderProgram::load_vertex_source(const char *source) {
    this->vShader = Ebb::Core::ShaderCompiler::compile_vertex(source);
}

void Ebb::Core::ShaderProgram::load_fragment_source(const char *source) {
    this->fShader = Ebb::Core::ShaderCompiler::compile_fragment(source);
}

void Ebb::Core::ShaderProgram::link() {
    glAttachShader(this->sProgram, this->vShader);
    glAttachShader(this->sProgram, this->fShader);
    glLinkProgram(this->sProgram);

    int success;
    glGetShaderiv(this->sProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(this->sProgram, 512, NULL, infoLog);
        fprintf(stderr, "Error linking shader program: \n%s\n\n", infoLog);
    }
}

void Ebb::Core::ShaderProgram::use() {
    glUseProgram(this->sProgram);
}
#include <ebb/core/shaders/shader.hpp>
#include <stdio.h>
#include <GL/glu.h>

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
    glGetProgramiv(this->sProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(this->sProgram, 512, NULL, infoLog);
        fprintf(stderr, "Error linking shader program: \n%s\n\n", infoLog);
    }
}

void Ebb::Core::ShaderProgram::set_bool(const char *name, bool value) {
    this->set_int(name, (int)value);
}
void Ebb::Core::ShaderProgram::set_int(const char *name, int value) {
    this->use();
    glUniform1i(glGetUniformLocation(this->sProgram, name), value);
}
void Ebb::Core::ShaderProgram::set_float(const char *name, float value) {
    this->use();
    glUniform1f(glGetUniformLocation(this->sProgram, name), value);
}

void Ebb::Core::ShaderProgram::set_floats(const char *name, float v0) {
    set_float(name, v0);
}
void Ebb::Core::ShaderProgram::set_floats(const char *name, float v0, float v1) {
    this->use();
    glUniform2f(glGetUniformLocation(this->sProgram, name), v0, v1);
}
void Ebb::Core::ShaderProgram::set_floats(const char *name, float v0, float v1, float v2) {
    this->use();
    glUniform3f(glGetUniformLocation(this->sProgram, name), v0, v1, v2);

}
void Ebb::Core::ShaderProgram::set_floats(const char *name, float v0, float v1, float v2, float v3) {
    this->use();
    glUniform4f(glGetUniformLocation(this->sProgram, name), v0, v1, v2, v3);
}

void Ebb::Core::ShaderProgram::set_float2(const char *name, float2 value) {
    set_floats(name, value.x, value.y);
}
void Ebb::Core::ShaderProgram::set_float3(const char *name, float3 value) {
    set_floats(name, value.x, value.y, value.z);
}
void Ebb::Core::ShaderProgram::set_float4(const char *name, float4 value) {
    set_floats(name, value.x, value.y, value.z, value.w);
}

void Ebb::Core::ShaderProgram::set_float3x3(const char *name, float3x3 value) {
    glUniformMatrix3fv(glGetUniformLocation(this->sProgram, name), 1, GL_FALSE, &value[0][0]);
}
void Ebb::Core::ShaderProgram::set_float4x4(const char *name, float4x4 value) {
    glUniformMatrix4fv(glGetUniformLocation(this->sProgram, name), 1, GL_FALSE, &value[0][0]);
}

void Ebb::Core::ShaderProgram::use() {
    glUseProgram(this->sProgram);
}
#include <ebb/core/shaders/shader.hpp>
#include <ebb/core/geometry/scene.hpp>
#include <ebb/core/shaders/light.hpp>
#include <vector>
#include <stdio.h>
#include <GL/glu.h>

static const char * const blinnPhongVertexSource = R"(

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 texCoord;

uniform mat4x4 projectionMatrix;
uniform mat4x4 viewMatrix;
uniform mat4x4 modelMatrix;

out vec3 normal;
out vec3 position;
out vec2 uv;

void main() {
    normal = mat3(modelMatrix[0].xyz, modelMatrix[1].xyz, modelMatrix[2].xyz) * aNorm;
    uv = texCoord;
    gl_Position = modelMatrix * (viewMatrix * projectionMatrix) * vec4(aPos, 0.0f);
    position = modelMatrix * aPos;
}

)";

static const char * const blinnPhongFragmentSource = R"(

#define MAX_LIGHTS 128;

#version 330 core
in vec3 normal;
in vec2 position;
in vec2 uv;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;
uniform float specular;

uniform int numLights;
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS]

void main() {
    vec3 finalDiffuse = vec3(0.0, 0.0, 0.0);
    vec3 finalSpecular = vec3(0.0, 0.0, 0.0);

    for (int lightIdx = 0; lightIdx < numLights; ++lightIdx) {
        vec3 dirToLight = normalize(lightPositions[lightIdx] - position)
        finalDiffuse += (lightColors[lightIdx] * diffuseColor) * max(0.1, dot(normal, dirToLight));

        vec3 halfway = normalize(normal + dirToLight);
        finalSpecular += (lightColors[lightIdx] * diffuseColor) * max(0.0, pow(1.0 - dot(halfway, normal), shininess));
    }

    FragColor = mix(finalDiffuse, finalSpecular, specular);
}

)";


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

Ebb::Core::Shaders::BlinnPhong::BlinnPhong(
        float3 diffuseColor, float3 specularColor,
        float shininess, float specular,
        Ebb::Core::Scene *scene) {

    this->diffuseColor = diffuseColor;
    this->specularColor = specularColor;
    this->shininess = shininess;
    this->specular = specular;
    this->scene = scene;

    this->program = new Ebb::Core::ShaderProgram();
    this->program->load_vertex_source(blinnPhongVertexSource);
    this->program->load_fragment_source(blinnPhongFragmentSource);
    this->program->link();
}

void Ebb::Core::Shaders::BlinnPhong::use() {
    this->program->use();

    this->program->set_float3("diffuseColor", diffuseColor);
    this->program->set_float3("specularColor", specularColor);

    this->program->set_float("shininess", shininess);
    this->program->set_float("specular", specular);

    int num_lights = this->scene->num_lights();
    std::vector<Ebb::Core::LightSource *> lights = this->scene->get_lights();
    float3 light_positions[num_lights];
    float3 light_colors[num_lights];

    for (int i = 0; i < num_lights; ++i) {
        light_positions[i] = lights[i]->position;
        light_colors[i]    = lights[i]->color * lights[i]->intensity;
    }


}
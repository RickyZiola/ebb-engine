#include <ebb/core/window/window.hpp>
#include <ebb/core/shaders/shader.hpp>
#include <ebb/core/geometry/mesh.hpp>
#include <ebb/math/vector.hpp>
#include <stdio.h>
#include <assert.h>
#include <chrono>
#include <math.h>
#include <GL/glu.h>

using Ebb::Core::Window;
using namespace Ebb::Math;

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 texCoord;

uniform float aspect;
uniform float time;

out vec2 uv;
out vec3 normal;
out vec3 pos;
void main() {
    uv = texCoord;
    normal = normalize(vec3((aNorm.x * cos(time) - aNorm.z * sin(time)), aNorm.y, (aNorm.x * sin(time) + aNorm.z * cos(time))));
    gl_Position = vec4((aPos.x * cos(time) - aPos.z * sin(time)), aPos.y, (aPos.x * sin(time) + aPos.z * cos(time)), 2.0) * .5;
    pos = gl_Position.xyz;
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 color;
uniform float time;

in vec2 uv;
in vec3 normal;
in vec3 pos;

void main() {
    vec3 light = normalize(vec3(1,1,-1));
    vec3 ambient = vec3(.1);
    vec3 h = normalize(normalize(pos) - light);
    FragColor = vec4(vec3(clamp(dot(normal, light), 0.0, 1.0)) + pow(max(dot(normal, h), 0.0), 16) * 0.5 + ambient, 1.0);
} 
)";

Ebb::Core::ShaderProgram *shader;
Ebb::Core::Mesh *mesh;
Ebb::Core::Window *window;
unsigned int VAO, VBO;

std::chrono::_V2::system_clock::time_point start_time;
unsigned long frames = 0;
float elapsed_seconds() {

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    return static_cast<float>(duration.count()) / 1'000'000.0f;
}

void frame_callback() {
    if(glfwGetKey(window->get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window->get_window(), true);

    float3 color = float3((std::sin(elapsed_seconds()) + 1.) / 2., (std::sin(elapsed_seconds() * 1.5) + 1.) / 2., (std::sin(elapsed_seconds() * 2.0) + 1.) / 2.);
    shader->set_float3("color", color);
    shader->set_float("time", elapsed_seconds() / 5);

    glClearColor(.5f, .8f, .9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mesh->draw();

    printf("FPS: %.01f                  \r", (float)frames++ / elapsed_seconds());
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    Ebb::Core::Internal::init_glfw();

    Window win = Window("Ebb::Core::Mesh test", &frame_callback);
    window = &win;
    
    shader = new Ebb::Core::ShaderProgram();
    shader->load_vertex_source(vertexShaderSource);
    shader->load_fragment_source(fragmentShaderSource);
    shader->link();

    shader->set_float("aspect", (float)win.get_width() / (float)win.get_height());

    mesh = Ebb::Core::Mesh::load_ebb_mesh("suzanne.ebbm", shader);

    start_time = std::chrono::high_resolution_clock::now();
    win.run();
    
    Ebb::Core::Internal::terminate_glfw();
    printf("\n\n");
    return 0;
}
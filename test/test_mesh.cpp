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
layout (location = 1) in vec2 texCoord;

uniform float aspect;

out vec2 uv;
void main() {
    uv = texCoord;
    gl_Position = vec4(aPos.x, aPos.yz, 1.0);
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 color;

in vec2 uv;
void main()
{
    FragColor = vec4(color * vec3(uv, 1.0), 1.0);
} 
)";

Ebb::Core::ShaderProgram *shader;
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

    float3 vertices[] = {
        // positions
        float3(-.5, -.5, 0.0), // bottom left
        float3(-.5,  .5, 0.0), // top left
        float3( .5, -.5, 0.0), // bottom right
        float3( .5,  .5, 0.0), // top right
    };

    float2 texCoords[] = {
        float2(0, 0), // Bottom left
        float2(0, 1), // Top left
        float2(1, 0), // Bottom right
        float2(1, 1), // Top right
    };
    
    unsigned int edgeTable[] = {
        0, 1, 2,
        3, 1, 2
    };

    Ebb::Core::Mesh mesh = Ebb::Core::Mesh(
        4, vertices,
        2, edgeTable,
        texCoords, shader);

    float3 color = float3((std::sin(elapsed_seconds()) + 1.) / 2., (std::sin(elapsed_seconds() * 1.5) + 1.) / 2., (std::sin(elapsed_seconds() * 2.0) + 1.) / 2.);
    shader->set_float3("color", color);

    glClearColor(.5f, .8f, .9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    mesh.draw();
    printf("FPS: %.01f                  \r", (float)frames++ / elapsed_seconds());
    fflush(stdout);
}
//
int main(int argc, char *argv[]) {
    Ebb::Core::Internal::init_glfw();

    Window win = Window("Ebb::Core::Mesh test", &frame_callback);
    window = &win;

    shader = new Ebb::Core::ShaderProgram();
    shader->load_vertex_source(vertexShaderSource);
    shader->load_fragment_source(fragmentShaderSource);
    shader->link();

    shader->set_float("aspect", (float)win.get_width() / (float)win.get_height());

    start_time = std::chrono::high_resolution_clock::now();
    win.run(960);
    
    Ebb::Core::Internal::terminate_glfw();
    printf("\n\n");
    return 0;
}
#include <ebb/core/window/window.hpp>
#include <ebb/core/shaders/shader.hpp>
#include <ebb/math/vector.hpp>
#include <stdio.h>
#include <assert.h>
#include <chrono>
#include <math.h>

using Ebb::Core::Window;
using namespace Ebb::Math;

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 texCoord;
uniform float aspectRatio;
uniform vec3 normal;

out vec3 uvCoord;
out vec3 triNormal;

void main() {
    mat4 projectionMatrix = mat4(
        1.0 / (aspectRatio * tan(radians(45.0 / 2.0))), 0.0, 0.0, 0.0,
        0.0, 1.0 / tan(radians(45.0 / 2.0)), 0.0, 0.0,
        0.0, 0.0, -1.001001001001001, -1.0,
        0.0, 0.0, -0.2002002002002002, 0.0
    );
    uvCoord = texCoord;
    triNormal = normal;
    gl_Position = vec4(aPos, 1.0);
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 color;
in vec3 uvCoord;
in vec3 triNormal;

void main()
{
    FragColor = vec4(color * uvCoord, 1.0);
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

    float vertices[] = {
        // positions         // UV coordinates (TODO: remove the blue/Z channel, mesh loading)
        0.5f  * std::cos(elapsed_seconds()), -0.5f, .5f * std::sin(elapsed_seconds()),   1.0f, 0.0f, 1.0f,   // bottom right
        -0.5f * std::cos(elapsed_seconds()), -0.5f,-.5f * std::sin(elapsed_seconds()),   0.0f, 0.0f, 1.0f,   // bottom left
        0.0f,                                 0.5f, 0.0f,                                0.0f, 1.0f, 1.0f    // top 
    };

    float3 norm12 = (float3(vertices[0], vertices[1], vertices[2]) - float3(vertices[6], vertices[7], vertices[8])).normalize();
    float3 norm13 = (float3(vertices[0], vertices[1], vertices[2]) - float3(vertices[12], vertices[13], vertices[14])).normalize();

    float3 normal = norm12.cross(norm13);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(VAO);

    float3 color = float3((std::sin(elapsed_seconds()) + 1.) / 2., (std::sin(elapsed_seconds() * 1.5) + 1.) / 2., (std::sin(elapsed_seconds() * 2.0) + 1.) / 2.);
    shader->set_float3("color", color);
    shader->set_float3("normal", normal);

    glClearColor(.5f, .8f, .9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    printf("FPS: %.01f                  \r", (float)frames++ / elapsed_seconds());
    fflush(stdout);
}
//
int main(int argc, char *argv[]) {
    Ebb::Core::Internal::init_glfw();

    Window win = Window("Ebb::Core::ShaderProgram test", &frame_callback);
    window = &win;

    shader = new Ebb::Core::ShaderProgram();
    shader->load_vertex_source(vertexShaderSource);
    shader->load_fragment_source(fragmentShaderSource);
    shader->link();

    shader->set_float("aspect", (float)win.get_width() / (float)win.get_height());

    shader->use();

    start_time = std::chrono::high_resolution_clock::now();
    win.run(960);
    
    Ebb::Core::Internal::terminate_glfw();
    printf("\n\n");
    return 0;
}
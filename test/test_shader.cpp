#include <ebb/core/window/window.hpp>
#include <ebb/core/shaders/shader.hpp>
#include <ebb/math/vector.hpp>
#include <stdio.h>
#include <assert.h>
#include <chrono>
#include <math.h>

using Ebb::Core::Window;
using namespace Ebb::Math;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 texCoord;\n"
    "out vec3 uvCoord;\n"
    "void main()\n"
    "{\n"
    "   uvCoord = texCoord.xyz;"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 color;
in vec3 uvCoord;

void main()
{
    FragColor = vec4(uvCoord, 1.0f);
} 
)";

Ebb::Core::ShaderProgram *shader;
unsigned int VAO, VBO;

std::chrono::_V2::system_clock::time_point start_time;
unsigned long frames = 0;
float elapsed_seconds() {

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    return static_cast<float>(duration.count()) / 1'000'000.0f;
}

void frame_callback() {
    float vertices[] = {
        // positions         // UV coordinates (TODO: remove the blue/Z channel, mesh loading)
        0.5f  * std::cos(elapsed_seconds()), -0.5f, .5f * std::sin(elapsed_seconds()),   1.0f, 0.0f, 1.0f,   // bottom right
        -0.5f * std::cos(elapsed_seconds()), -0.5f,-.5f * std::sin(elapsed_seconds()),   0.0f, 0.0f, 1.0f,   // bottom left
        0.0f,                                 0.5f, 0.0f,                                0.0f, 1.0f, 1.0f    // top 
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(VAO);

    float3 color = float3((std::sin(elapsed_seconds()) + 1.) / 2., (std::sin(elapsed_seconds() * 1.5) + 1.) / 2., (std::sin(elapsed_seconds() * 2.0) + 1.) / 2.);
    shader->set_float3("color", color);

    glClearColor(.5f, .8f, .9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    printf("FPS: %f                  \r", (float)frames++ / elapsed_seconds());
    fflush(stdout);
}
//
int main(int argc, char *argv[]) {
    Ebb::Core::Internal::init_glfw();

    Window win = Window(800,600, "Ebb::Core::ShaderProgram test", &frame_callback);

    shader = new Ebb::Core::ShaderProgram();
    shader->load_vertex_source(vertexShaderSource);
    shader->load_fragment_source(fragmentShaderSource);
    shader->link();

    shader->use();

    start_time = std::chrono::high_resolution_clock::now();
    win.run(960);
    
    Ebb::Core::Internal::terminate_glfw();
    return 0;
}
#include <ebb/core/window/window.hpp>
#include <ebb/core/shaders/shader.hpp>
#include <stdio.h>
#include <assert.h>
#include <chrono>

using Ebb::Core::Window;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
)";

std::chrono::_V2::system_clock::time_point start_time;
unsigned long frames = 0;
float elapsed_seconds() {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    return static_cast<float>(duration.count()) / 1'000'000.0f;
}

void frame_callback() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    printf("FPS: %f                  \r", (float)frames++ / elapsed_seconds());
    fflush(stdout);
}
//
int main(int argc, char *argv[]) {
    Ebb::Core::Internal::init_glfw();

    Window win = Window(800,600, "Ebb::Core::ShaderProgram test", &frame_callback);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    

    Ebb::Core::ShaderProgram shader = Ebb::Core::ShaderProgram();
    shader.load_vertex_source(vertexShaderSource);
    shader.load_fragment_source(fragmentShaderSource);
    shader.link();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 
    glBindVertexArray(VAO);

    shader.use();

    start_time = std::chrono::high_resolution_clock::now();
    win.run(480);
    
    Ebb::Core::Internal::terminate_glfw();
    return 0;
}
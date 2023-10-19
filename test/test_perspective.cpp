#include <ebb/core/window/window.hpp>
#include <ebb/core/shaders/shader.hpp>
#include <ebb/core/geometry/mesh.hpp>
#include <ebb/core/shaders/texture.hpp>
#include <ebb/math/math.hpp>
#include <stdio.h>
#include <assert.h>
#include <chrono>
#include <math.h>
#include <GL/glu.h>

using Ebb::Core::Window;
using namespace Ebb::Math;

Ebb::Core::ShaderProgram *shader;
Ebb::Core::Mesh *mesh;
Ebb::Core::Window *window;
unsigned int VAO, VBO;
float aspect;

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

    float time = elapsed_seconds();

    float3 color = float3((std::sin(time * 2.) + 1.) / 2., (std::sin(time * 3.) + 1.) / 2., (std::sin(time * 4.0) + 1.) / 2.);
    shader->set_float3("color", color);
    shader->set_float("time", time / 5);

    float4x4 cameraMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f);
    shader->set_float4x4("projectionMatrix", cameraMatrix);

    float4x4 viewMatrix = glm::lookAt(float3(2.0f * cos(time / 2.0f), 2.0f, 2.0f * sin(time / 2.0f)), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f));
    shader->set_float4x4("viewMatrix", viewMatrix);

    glClearColor(.5f, .8f, .9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mesh->draw();

    printf("FPS: %.01f                  \r", (float)frames++ / time);
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    Ebb::Core::Internal::init_glfw();

    Window win = Window("Perspective camera test", &frame_callback);
    window = &win;
    
    shader = new Ebb::Core::TextureShader("textures/companion-cube.png");

    aspect = (float)win.get_width() / (float)win.get_height();

    mesh = Ebb::Core::Mesh::load_ebb_mesh("models/cube.ebbm", shader);

    start_time = std::chrono::high_resolution_clock::now();
    win.run();
    
    Ebb::Core::Internal::terminate_glfw();
    printf("\n\n");
    return 0;
}
#include <ebb/core/window/window.hpp>
#include <ebb/core/shaders/shader.hpp>
#include <ebb/core/geometry/mesh.hpp>
#include <ebb/core/shaders/texture.hpp>
#include <ebb/math/vector.hpp>
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

    float3 color = float3((std::sin(elapsed_seconds() * 2.) + 1.) / 2., (std::sin(elapsed_seconds() * 3.) + 1.) / 2., (std::sin(elapsed_seconds() * 4.0) + 1.) / 2.);
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
    
    shader = new Ebb::Core::TextureShader("textures/test_texture.jpg");

    shader->set_float("aspect", (float)win.get_width() / (float)win.get_height());

    mesh = Ebb::Core::Mesh::load_ebb_mesh("models/cube.ebbm", shader);

    start_time = std::chrono::high_resolution_clock::now();
    win.run();
    
    Ebb::Core::Internal::terminate_glfw();
    printf("\n\n");
    return 0;
}
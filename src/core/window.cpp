#include <ebb/core/window/window.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

static void _window_size_callback(GLFWwindow *window, int width, int height) {
    Ebb::Core::Window* instance = reinterpret_cast<Ebb::Core::Window*>(glfwGetWindowUserPointer(window));
    instance->_window_resize_callback(window, width, height);
}

Ebb::Core::Window::Window(int width, int height, const char *title) {
    this->width = width;
    this->height = height;

    this->window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (this->window == nullptr) {
        perror("Failed to create GLFW window.\n");
        exit(-1);
    }

    glfwSetWindowUserPointer(this->window, this);
    glfwMakeContextCurrent(this->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        perror("Failed to initialize GLAD.\n");
        exit(-1);
    }
    
    glViewport(0, 0, width, height);
    glfwSetWindowSizeCallback(this->window, _window_size_callback);

    this->render_loop_callback = nullptr;
}

Ebb::Core::Window::Window(int width, int height, const std::string& title) : Window(width, height, title.c_str()) {}

Ebb::Core::Window::Window(int width, int height, const std::string& title, RenderLoopCallback callback) : Window(width, height, title.c_str()) {
    this->render_loop_callback = callback;
}

Ebb::Core::Window::Window(int width, int height, const char *title, RenderLoopCallback callback) : Window(width, height, title) {
    this->render_loop_callback = callback;
}

Ebb::Core::Window::Window(const char *title) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    int width   = mode->width;
    int height = mode->height;

    this->width = width;
    this->height = height;

    this->window = glfwCreateWindow(width, height, title, monitor, NULL);
    if (this->window == nullptr) {
        perror("Failed to create GLFW window.\n");
        exit(-1);
    }

    glfwSetWindowUserPointer(this->window, this);
    glfwMakeContextCurrent(this->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        perror("Failed to initialize GLAD.\n");
        exit(-1);
    }

    glViewport(0, 0, width, height);
    glfwSetWindowSizeCallback(this->window, _window_size_callback);

    this->render_loop_callback = nullptr;
}
Ebb::Core::Window::Window(const std::string& title) : Window(title.c_str()) {}
Ebb::Core::Window::Window(const std::string& title, RenderLoopCallback callback) : Window(title) {
    this->render_loop_callback = callback;
}

Ebb::Core::Window::Window(const char *title, RenderLoopCallback callback) : Window(title) {
    this->render_loop_callback = callback;
}

void Ebb::Core::Window::_window_resize_callback(GLFWwindow *window, int width, int height) {
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
    this->width = width;
    this->height = height;
}

void Ebb::Core::Window::run(unsigned long long frames) {
    for (; frames > 0 && !glfwWindowShouldClose(this->window); --frames) {
        step();
    }
}

void Ebb::Core::Window::step() {
    glfwMakeContextCurrent(this->window);
    glfwSwapBuffers(window);
    glfwPollEvents();
    if (this->render_loop_callback != nullptr) this->render_loop_callback();
}

void Ebb::Core::Window::make_active() {
    glfwMakeContextCurrent(this->window);
}
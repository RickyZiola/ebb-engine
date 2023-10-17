#include <ebb/core/window/window.hpp>
#include <stdbool.h>

static bool _glfwInitialized = false;

void Ebb::Core::Internal::init_glfw() {
    if (_glfwInitialized) return;
    _glfwInitialized = true;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwSwapInterval(1);
}

void Ebb::Core::Internal::terminate_glfw() {
    glfwTerminate();
}
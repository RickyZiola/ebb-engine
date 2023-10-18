#include <ebb/core/window/window.hpp>
#include <stdio.h>
#include <assert.h>

using Ebb::Core::Window;

int frames_run = 0;

void render_loop(void) {
    frames_run++;
}

void pre_loop(void) {
    frames_run = -1;
}

int main(int argc, char *argv[]) {
    Ebb::Core::Internal::init_glfw();

    Window win = Window(800,600, "Test Ebb::Core::Window", &pre_loop);
    win.step();

    assert(frames_run == -1);

    win.set_render_loop_callback(&render_loop);
    frames_run = 0;

    win.run(60);

    assert(frames_run == 60);

    printf("Test Ebb::Core::Window passed!\n\n");

    Ebb::Core::Internal::terminate_glfw();
    return 0;
}
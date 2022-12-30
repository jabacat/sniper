/* Main file for Sniper program. */

#include <chrono>
#include <iostream>
#include <thread>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <graphics/shader.hpp>
#include <graphics/texture.hpp>

namespace sniper {

GLFWwindow *wn;

void init() {

    glfwInit(); // TODO check for error

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    wn = glfwCreateWindow(500, 500, "Sniper", nullptr, nullptr);
    glfwMakeContextCurrent(wn);
    gladLoadGL();
    glfwShowWindow(wn);

    gl::load_all_shaders();
    tex::load_all_textures();
}

void mainloop() {

    tex::Texture player_tex{tex::GAME_TEX, 0.0, 0.75, 0.25, 0.25};

    gl::GAME_SHADER->bind();

    while (!glfwWindowShouldClose(wn)) {

        // We want each frame to last for exactly 1/50th second,
        // so capture the starting time so we can sleep for the
        // needed amount of time at the end of the frame.
        auto start_of_frame = std::chrono::steady_clock::now();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        player_tex.render(0, 0, 0.2, 0.2, tex::RenderBasis::MID, tex::RenderBasis::MID);

        glfwSwapBuffers(wn);
        glfwPollEvents();

        std::this_thread::sleep_until(start_of_frame + std::chrono::milliseconds(20));
    }

    gl::GAME_SHADER->unbind();
}

void cleanup() {

    tex::unload_all_textures();
    gl::unload_all_shaders();

    glfwDestroyWindow(wn);
    glfwTerminate();
}

} // namespace sniper

int main() {

    sniper::init();
    sniper::mainloop();
    sniper::cleanup();

    return 0;
}

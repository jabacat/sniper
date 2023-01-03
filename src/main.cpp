/* Main file for Sniper program. */

#include <chrono>
#include <iostream>
#include <math.h>
#include <thread>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <game/player.hpp>
#include <graphics/shader.hpp>
#include <graphics/texture.hpp>

namespace sniper {

GLFWwindow * wn;

/* Maximum possible speeds for the player. */
float MAX_X = 0.5f, MAX_Y = 0.5f;

/* How slowly (the inverse of how fast in the internal calculation) the player's
speed updates to the maximum it can be in the direction they are traveling. */
float SLIPPERINESS = 25;

/* How much friction there is. */
float FRICTION = 0.01f;

/* FPS. */
int FPS = 50;

/* Get the new X and Y speed of the player, given the current X and Y speed and
readings from GLFW of the currently pressed keys. */
std::pair<float, float> new_speed(std::pair<float, float> old_speed) {

    std::pair<float, float> ret = old_speed;

    // For horizontal and vertical axes, we return -1, 0, or 1 -- which
    // direction in each to increase speed in.
    int hinc = 0, vinc = 0;

    if (glfwGetKey(wn, GLFW_KEY_A) == GLFW_PRESS) {
        hinc -= 1;
    }
    if (glfwGetKey(wn, GLFW_KEY_D) == GLFW_PRESS) {
        hinc += 1;
    }
    if (glfwGetKey(wn, GLFW_KEY_W) == GLFW_PRESS) {
        vinc += 1;
    }
    if (glfwGetKey(wn, GLFW_KEY_S) == GLFW_PRESS) {
        vinc -= 1;
    }

    /* We slow down a tiny bit if nothing is pressed. */
    if (hinc == 0 && vinc == 0) {
        ret.first  *= (1.0f - FRICTION);
        ret.second *= (1.0f - FRICTION);
        return ret;
    }

    // The division by the sum is so that the maximum TOTAL velocity is the same
    // even if both horizontal and vertical velocity components are changing.
    float div = sqrt(hinc * hinc + vinc * vinc);
    float ideal_x = MAX_X * hinc / (div * FPS);
    float ideal_y = MAX_Y * vinc / (div * FPS);

    float update_frac = 1 / SLIPPERINESS;

    ret.first  = ret.first  * (1 - update_frac) + ideal_x * update_frac;
    ret.second = ret.second * (1 - update_frac) + ideal_y * update_frac;

    return ret;

}

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

    game::Player player(0, 0);
    
    gl::GAME_SHADER->bind();

    while (!glfwWindowShouldClose(wn)) {

		// We want each frame to last for exactly 1/50th second,
		// so capture the starting time so we can sleep for the
		// needed amount of time at the end of the frame.
		auto start_of_frame = std::chrono::steady_clock::now();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update the player speed and position.
        player.setvel(new_speed(player.getvel()));
        player.do_pos_updates();

        player.render();

		glfwSwapBuffers(wn);
		glfwPollEvents();

		std::this_thread::sleep_until(start_of_frame + std::chrono::milliseconds(1000 / FPS));

	}

    gl::GAME_SHADER->unbind();

}

void cleanup() {

    tex::unload_all_textures();
    gl::unload_all_shaders();

    glfwDestroyWindow(wn);
	glfwTerminate();

}

}

int main() {
    
    sniper::init();
    sniper::mainloop();
    sniper::cleanup();

    return 0;

}

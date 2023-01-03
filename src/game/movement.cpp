#include "movement.hpp"

#include <math.h>

namespace mvmt {

/* Maximum possible speeds for the player. */
float MAX_X = 0.5f, MAX_Y = 0.5f;

/* How slowly (the inverse of how fast in the internal calculation) the player's
speed updates to the maximum it can be in the direction they are traveling. */
float SLIPPERINESS = 25;

/* How much friction there is. */
float FRICTION = 0.01f;

/* Get the new X and Y speed of the player, given the current X and Y speed and
readings from GLFW of the currently pressed keys. */
std::pair<float, float> new_speed(GLFWwindow * wn, std::pair<float, float> old_speed) {

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

int FPS = 50;

} // namespace mvmt

namespace game {

void Player::do_bounce(float screen_edge) {

    if (x > screen_edge || x < -screen_edge) {
        vx *= -1;
    }
    if (y > screen_edge || y < -screen_edge) {
        vy *= -1;
    }

}

}

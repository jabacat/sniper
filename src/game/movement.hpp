#pragma once

/* This file has utilities for controlling the player's movement. */

#include <vector>

#include <GLFW/glfw3.h>

#include <game/player.hpp>
#include <game/bullet.hpp>

namespace mvmt {

/* Get the new X and Y speed of the player, given the current X and Y speed and
readings from GLFW of the currently pressed keys. */
std::pair<float, float> new_speed(GLFWwindow * wn, std::pair<float, float> old_speed);

/* FPS. */
extern int FPS;

}

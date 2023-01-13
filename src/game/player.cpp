#include "player.hpp"
#include <cmath>

#include <graphics/texture.hpp>

namespace game {

static float PLAYER_SIZE = 0.2f;

Player::Player(int xloc, int yloc) {
    this->x = xloc;
    this->y = yloc;
    this->vx = 0;
    this->vy = 0;
    this->angle;
}

void Player::render() {
    tex::PLAYER_TEX->render(this->x, this->y, PLAYER_SIZE, PLAYER_SIZE,
                            tex::RenderBasis::MID, tex::RenderBasis::MID);
}

std::pair<float, float> Player::getvel() {
    return std::pair(this->vx, this->vy);
}

void Player::setvel(std::pair<float, float> newvel) {
    this->vx = newvel.first;
    this->vy = newvel.second;

    const float PI = 3.14159265;
    const float DEG_CONV = 180.0 / PI;

    float x, y, m;
    x = fabs(this->vx);
    y = fabs(this->vy);

    // Normalize the vector (x, y)
    float r = sqrt(y * y + x * x);
    x /= r;
    y /= r;

    // arccos approximation [-1, 1]
    // m = -1.4 * x + 1.55;

    // arccos approximation [0, 1]
    // This one will be used because x is in the domain [0, 1]
    // err = integral 0->1 arccos(x) - f(x) dx = -0.01
    m = -1.12 * x + 1.57;
    m *= DEG_CONV;

    setangle(m);
}

void Player::setangle(float a) { this->angle = a; }

} // namespace game

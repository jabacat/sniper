#include "player.hpp"

#include <graphics/texture.hpp>

namespace game {

static float PLAYER_SIZE = 0.2f;

Player::Player(int xloc, int yloc) {
    this->x = xloc;
    this->y = yloc;
    this->vx = 0;
    this->vy = 0;
}

void Player::render() {
    tex::PLAYER_TEX->render(this->x, this->y, PLAYER_SIZE, PLAYER_SIZE, tex::RenderBasis::MID, tex::RenderBasis::MID);
}

std::pair<float, float> Player::getvel() {
    return std::pair(this->vx, this->vy);
}

void Player::setvel(std::pair<float, float> newvel) {
    this->vx = newvel.first;
    this->vy = newvel.second;
}

void Player::do_bounce(float screen_edge) {

    if (x > screen_edge || x < -screen_edge) {
        vx *= -1;
    }
    if (y > screen_edge || y < -screen_edge) {
        vy *= -1;
    }

}

}

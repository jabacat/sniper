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

}

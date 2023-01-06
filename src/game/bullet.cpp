#include "bullet.hpp"

#include <graphics/texture.hpp>

namespace game {

Bullet::Bullet(float xloc, float yloc, float angle) {
    this->x = xloc;
    this->y = yloc;
    this->angle = angle;
    this->vx = 0;
    this->vy = 0;
}

void Bullet::render() {
    // TODO: Change to actual bullet texture
    tex::PLAYER_TEX->render(this->x, this->y, 0.01f, 0.04f,
                            tex::RenderBasis::MID, tex::RenderBasis::MID);
}

std::pair<float, float> Bullet::getvel() {
    return std::pair(this->vx, this->vy);
}

void Bullet::setvel(std::pair<float, float> newvel) {
    this->vx = newvel.first;
    this->vy = newvel.second;
}

} // namespace game

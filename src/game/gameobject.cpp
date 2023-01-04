#include "gameobject.hpp"

namespace game {

void GameObject::set_pos(int x_pos, int y_pos) {
    // TODO check bounds

    x = x_pos;
    y = y_pos;
}

void GameObject::do_pos_updates() {
    x += vx;
    y += vy;
}

} // namespace game

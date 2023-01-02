#pragma once

#include <game/gameobject.hpp>
#include <game/renderable.hpp>

namespace game {

class Player : GameObject, Renderable {

  public:
    void render() override;
    Player(int xloc, int yloc);

};

} // namespace

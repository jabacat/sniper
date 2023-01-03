#pragma once

#include <vector>

#include <game/gameobject.hpp>
#include <game/renderable.hpp>

namespace game {

class Player : public GameObject, Renderable {

  public:
    void render() override;
    Player(int xloc, int yloc);

    // Read and update velocities.
    std::pair<float, float> getvel();
    void setvel(std::pair<float, float> newvel);

};

} // namespace

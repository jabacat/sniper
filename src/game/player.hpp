#pragma once

#include <vector>
#include <iostream>

#include <game/gameobject.hpp>
#include <game/renderable.hpp>

namespace game {

class Player : public GameObject, Renderable {

  public:
    void render() override;
    Player(int xloc, int yloc);

    /* Bounce off the edges of the screen. */
    void do_bounce(float screen_edge);

    // Read and update velocities.
    std::pair<float, float> getvel();
    void setvel(std::pair<float, float> newvel);
    void setangle(float a);

  protected:
    float angle;
};

} // namespace game

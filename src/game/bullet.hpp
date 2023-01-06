#pragma once

#include <vector>

#include <game/gameobject.hpp>
#include <game/renderable.hpp>

namespace game {

class Bullet : public GameObject, Renderable {

  public:
    void render() override;
    Bullet(float xloc, float yloc, float angle);

	float angle;

    /* Bounce off the edges of the screen. */
    void do_bounce(float screen_edge);

    // Read and update velocities.
    std::pair<float, float> getvel();
    void setvel(std::pair<float, float> newvel);
};

} // namespace game

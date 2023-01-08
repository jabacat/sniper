#include "background.hpp"

#include <graphics/texture.hpp>

namespace game {

void Background::render() {
    tex::BG_TEX->render(0, 0, 1.0, 1.0, tex::RenderBasis::MID,
                        tex::RenderBasis::MID);
}

} // namespace game

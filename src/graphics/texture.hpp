/**
 * One texture.
 * TODO - texture atlasing perhaps?
 */

#pragma once

#include <memory>
#include <system/string.hpp>

#include <glad/glad.h>

namespace gl {

class TextureAtlas {

    // The OpenGL ID assigned to this texture.
    unsigned id = 0;

protected:
    // Load a buffer of width w and height h, of format f, into a GL texture and
    // return the ID.
    static unsigned load(const unsigned char* buffer, int w, int h, int f);

public:
    TextureAtlas(ZStringView filename);
    TextureAtlas() = default;
   ~TextureAtlas();
    void bind(); // Make this the currently drawn texture
    void unbind(); // Shouldn't need to be used often, actually
    void destroy();

};

}

namespace tex {

/**
 * What part of the texture the coordinates reference. e.g.: LOW, CENTER means
 * the x coordinate is the left of the figure and the y coordinate is the middle
 * of it.
 */
enum RenderBasis {
    LOW,
    MID,
    HI
};

class Texture {

private:

    // The texture atlas that this texture is part of.
    std::shared_ptr<gl::TextureAtlas> atlas;

    double start_x, start_y, w, h;

public:

    Texture(std::shared_ptr<gl::TextureAtlas> a, double sx, double sy, double w, double h);

    void render(double x, double y, double width, double height, RenderBasis xbasis, RenderBasis ybasis);

};

// More textures here later?
extern std::shared_ptr<gl::TextureAtlas>
    GAME_TEX;

extern std::shared_ptr<tex::Texture> PLAYER_TEX;

static std::shared_ptr<gl::TextureAtlas> _BACKGROUND_TEX;
extern std::shared_ptr<tex::Texture> BG_TEX;

void load_all_textures();
void unload_all_textures();

}

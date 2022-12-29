/**
 * One texture.
 * TODO - texture atlasing perhaps?
 */

#pragma once

#include <memory>
#include <string>

#include <glad/glad.h>

namespace gl {

class TextureAtlas {

    // The OpenGL ID assigned to this texture.
    unsigned id;

protected:
    // Load a buffer of width w and height h, of format f, into a GL texture and
    // return the ID.
    static unsigned load(const unsigned char* buffer, int w, int h, int f);

public:
    TextureAtlas(std::string filename);
    TextureAtlas() = default;
   ~TextureAtlas();
    void bind(); // Make this the currently drawn texture
    void unbind(); // Shouldn't need to be used often, actually
    void destroy();

};

}

namespace tex {

// More textures here later?
extern std::unique_ptr<gl::TextureAtlas>
    GAME_TEX;

void load_all_textures();
void unload_all_textures();

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

}

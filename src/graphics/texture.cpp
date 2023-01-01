#include "texture.hpp"

#include <graphics/stb_image.h>
#include <graphics/mesh.hpp>

namespace gl {

TextureAtlas::TextureAtlas(std::string name) {

    stbi_set_flip_vertically_on_load(true);

    // STB stuffs these with data
    int w, h, channels;
    auto data = stbi_load(name.c_str(), &w, &h, &channels, 0);
    if (!data)
        ; // TODO - error out

    this->id = load(data, w, h, GL_RGBA);

    stbi_image_free(data);

}

unsigned TextureAtlas::load(const unsigned char* buffer, int w, int h, int f) {

    unsigned ret;

    glGenTextures(1, &ret);
    glBindTexture(GL_TEXTURE_2D, ret);

    // It's a pixel art game, we don't want the textures to appear blurry
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, f, w, h, 0, f, GL_UNSIGNED_BYTE, buffer);

    glGenerateMipmap(GL_TEXTURE_2D);

    return ret;

}

void TextureAtlas::destroy() {
    if (this->id) {
        glDeleteTextures(1, &this->id);
        this->id = 0;
    }
}

TextureAtlas::~TextureAtlas() {
    this->destroy();
}

void TextureAtlas::bind() {
    glBindTexture(GL_TEXTURE_2D, this->id);
}

void TextureAtlas::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

}

namespace tex {

std::shared_ptr<gl::TextureAtlas> GAME_TEX;
std::shared_ptr<tex::Texture> PLAYER_TEX;

void load_all_textures() {
    GAME_TEX = std::make_shared<gl::TextureAtlas>("assets/texture-atlas.png");
    PLAYER_TEX = std::make_shared<tex::Texture>(tex::GAME_TEX, 0.0, 0.75, 0.25, 0.25);
}

void unload_all_textures() {
    GAME_TEX->destroy();
}

// Private -- render a texture atlas
void render_texture(
    double x, double y, double width, double height,
    RenderBasis xbasis, RenderBasis ybasis, std::shared_ptr<gl::TextureAtlas> tex,
    float x_start, float y_start, float w, float h
) {
    float x_off, y_off;
    switch (xbasis) {
        case LOW:
            x_off = 0;
            break;
        case MID:
            x_off = - width / 2;
            break;
        case HI:
            x_off = - width;
            break;
    }
    switch (ybasis) {
        case LOW:
            y_off = 0;
            break;
        case MID:
            y_off = - height / 2;
            break;
        case HI:
            y_off = - height;
            break;
    }
    float
        xb = (float) x + x_off,
        yb = (float) y + y_off,
        wb = (float) width,
        hb = (float) height;
    Mesh mesh {
        std::vector<float>{
            xb,      yb,
            xb,      yb + hb,
            xb + wb, yb + hb,
            xb + wb, yb
        },
        std::vector<int>{0, 1, 3, 3, 1, 2},
        std::vector<float>{
            x_start,     y_start    ,
            x_start,     y_start + h,
            x_start + w, y_start + h,
            x_start + w, y_start
        }
    };
    tex->bind();
    mesh.draw();
}

Texture::Texture(std::shared_ptr<gl::TextureAtlas> a, double sx, double sy, double w, double h) {
    atlas = a;
    start_x = sx;
    start_y = sy;
    this->w = w;
    this->h = h;
}

void Texture::render(double x, double y, double width, double height, RenderBasis xbasis, RenderBasis ybasis) {
    render_texture(x, y, width, height, xbasis, ybasis, atlas, start_x, start_y, w, h);
}

}
#include "shader.hpp"

#include <iostream>
#include <stdio.h>
#include <string>

#include <glad/glad.h>
#include <system/file.hpp>

namespace gl {

#ifndef __APPLE__
static std::string_view bytes_as_string_view(const std::vector<unsigned char>& vec) {
    return std::string_view(reinterpret_cast<const char*>(vec.data()), vec.size());
}
#else
// Load a whole file as a string.
const GLchar* load_file_as_string(const char* path) {
    // TODO - do this with C++ classes.
    FILE* fp = fopen(path, "r");
    if (!fp) return nullptr;
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    // Yes, I know the memes. Haha. Funny.
    char* code = (char*) malloc(len + 1);
    rewind(fp);
    for (int pos = 0; pos < len; ++pos) {
        code[pos] = getc(fp);
    }
    fclose(fp);
    code[len] = 0;
    return code;
}
#endif // __APPLE__

Shader::Shader(ZStringView vertex_fname, ZStringView fragment_fname) {

#ifndef __APPLE__
    std::vector<unsigned char> vertex_source = load_file_as_bytes(vertex_fname);
    std::vector<unsigned char> fragment_source = load_file_as_bytes(fragment_fname);
#else
    auto vertex_source = load_file_as_string(vertex_fname);
    auto fragment_source = load_file_as_string(fragment_fname);
#endif // __APPLE__

    this->program_id = glCreateProgram();
    // TODO - check for error

    int vert = create_subshader(vertex_source, GL_VERTEX_SHADER);
    int frag = create_subshader(fragment_source, GL_FRAGMENT_SHADER);

    this->link(vert, frag);

#ifdef __APPLE__
    free((void *) vertex_source);
    free((void *) fragment_source);
#endif // __APPLE__

}

int Shader::create_subshader(FileContents source, GLenum type) {

    int id = glCreateShader(type);
    if (!id) {
        // TODO -- log some error
    }

#ifndef __APPLE__
    GLchar const * src = source.data();
    int len = source.size();
#else
    GLchar const * src = source;
    int len = strlen(source);
#endif // __APPLE__
    glShaderSource(id, 1, &src, &len);
    glCompileShader(id);

    GLint compile_status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE) {
        // Check for error - if so, ...
        GLchar errorbuf[512];
        glGetShaderInfoLog(id, 512, nullptr, errorbuf);
        std::cerr << std::string(errorbuf) << "\n";
    }

    return id;

}

void Shader::link(int vert, int frag) {
    glAttachShader(this->program_id, vert);
    glAttachShader(this->program_id, frag);
    glLinkProgram (this->program_id);
    // We no longer need these sub-shaders.
    glDetachShader(this->program_id, vert);
    glDetachShader(this->program_id, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader::bind() {
    glUseProgram(this->program_id);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::destroy() {
    if (this->program_id) {
        glDeleteShader(this->program_id);
        this->program_id = 0;
    }
}

Shader::~Shader() {
    this->unbind();
    glDeleteShader(this->program_id);
}

std::unique_ptr<Shader> GAME_SHADER, TEXT_SHADER;

void Shader::register_uniform(std::string name) {
    int loc = glGetUniformLocation(this->program_id, name.c_str());
    if (loc < 0) {
        // TODO -- log error
    }
    this->uniforms[name] = loc;
}

void Shader::set_uniform_value(std::string name, float value) {
    glUniform1f(this->uniforms[name], value);
}

void load_all_shaders() {
    GAME_SHADER = std::make_unique<Shader>(
        "src/shader/basic_vert.glsl", "src/shader/basic_frag.glsl"
    );
}

void unload_all_shaders() {
    GAME_SHADER->destroy();
}

}

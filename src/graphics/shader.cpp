#include "shader.hpp"

#include <iostream>
#include <stdio.h>
#include <string>

#include <glad/glad.h>
#include <system/file.hpp>

namespace gl {

static std::string_view bytes_as_string_view(const std::vector<unsigned char>& vec) {
    return std::string_view(reinterpret_cast<const char *>(vec.data()), vec.size());
}

Shader::Shader(std::string_view vertex_source, std::string_view fragment_source) {

    this->program_id = glCreateProgram();
    // TODO - check for error

    int vert = create_subshader(vertex_source, GL_VERTEX_SHADER);
    int frag = create_subshader(fragment_source, GL_FRAGMENT_SHADER);

    this->link(vert, frag);
}

Shader Shader::load_from_file(ZStringView vertex_fname, ZStringView fragment_fname) {
    std::vector<unsigned char> vertcode = load_file_as_bytes(vertex_fname);
    std::vector<unsigned char> fragcode = load_file_as_bytes(fragment_fname);
    return Shader(bytes_as_string_view(vertcode), bytes_as_string_view(fragcode));
}

int Shader::create_subshader(std::string_view source, GLenum type) {

    int id = glCreateShader(type);
    if (!id) {
        // TODO -- log some error
    }

    const char * src = source.data();
    int len = source.size();
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
    glLinkProgram(this->program_id);
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
    // clang-format off
    GAME_SHADER = std::make_unique<Shader>(Shader::load_from_file(
        "src/shader/basic_vert.glsl", "src/shader/basic_frag.glsl"));
    // clang-format on
}

void unload_all_shaders() {
    GAME_SHADER->destroy();
}

} // namespace gl

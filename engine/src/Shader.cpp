#include "Shader.h"

#include <cmrc/cmrc.hpp>
#include <iostream>
#include <string>

#include "Log.h"
CMRC_DECLARE(forge);

namespace Forge {

// Helper to load embedded file as a string
static std::string loadEmbeddedFile(const std::string& path) {
    auto fs = cmrc::forge::get_filesystem();
    auto file = fs.open(path);
    std::string src(file.begin(), file.end());
    if (src.size() >= 3 && (unsigned char)src[0] == 0xEF &&
        (unsigned char)src[1] == 0xBB && (unsigned char)src[2] == 0xBF) {
        src = src.substr(3);  // Remove UTF-8 BOM if present
    }
    return src;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertSrc = loadEmbeddedFile(vertexPath);
    std::string fragSrc = loadEmbeddedFile(fragmentPath);

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragSrc);

    if (vertexShader == 0 || fragmentShader == 0) {
        FORGE_ERROR("Failed to compile shaders.");

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    // Link shaders into a program
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    // Check for linking errors
    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        FORGE_ERROR("Shader program linking failed:\n" + std::string(infoLog));

        glDeleteProgram(programID);
        programID = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    if (programID) {
        glDeleteProgram(programID);
    }
}

Shader::Shader(Shader&& other) noexcept : programID(other.programID) {
    other.programID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    if (programID) {
        glDeleteProgram(programID);
    }

    programID = other.programID;
    other.programID = 0;
    return *this;
}

void Shader::bind() const {
    if (programID) {
        glUseProgram(programID);
    }
}
void Shader::unbind() const { glUseProgram(0); }

GLuint Shader::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* srcPtr = source.c_str();
    glShaderSource(shader, 1, &srcPtr, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        FORGE_ERROR("Shader compilation failed:\n" + std::string(infoLog));
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
}  // namespace Forge

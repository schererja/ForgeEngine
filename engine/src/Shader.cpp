#include "Shader.h"
#include <iostream>

namespace Forge {
Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexPath);
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentPath);

  if (vertexShader == 0 || fragmentShader == 0) {
    std::cerr << "[Forge] Failed to compile shaders." << std::endl;
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
    std::cerr << "[Forge] Shader program linking failed:\n"
              << infoLog << std::endl;
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

Shader::Shader(Shader &&other) noexcept : programID(other.programID) {
  other.programID = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept {
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

GLuint Shader::compileShader(GLenum type, const std::string &source) {
  GLuint shader = glCreateShader(type);
  const char *srcPtr = source.c_str();
  glShaderSource(shader, 1, &srcPtr, nullptr);
  glCompileShader(shader);

  // Check for compilation errors
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cerr << "[Forge] Shader compilation failed:\n" << infoLog << std::endl;
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}
} // namespace Forge

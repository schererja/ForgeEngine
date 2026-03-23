#pragma once

#include <glad/gl.h>

#include <string>

namespace Forge {

// Owns an OpenGL shader program and manages bind/unbind lifecycle.
class Shader {
public:
  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  ~Shader();

  // Non-copyable because program ownership is unique.
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;

  // Movable to transfer program ownership.
  Shader(Shader &&other) noexcept;
  Shader &operator=(Shader &&other) noexcept;

  void bind() const;
  void unbind() const;

  bool isValid() const { return programID != 0; }

private:
  GLuint programID = 0;

  // Compiles a single shader stage and returns its object ID.
  GLuint compileShader(GLenum type, const std::string &source);
};
} // namespace Forge
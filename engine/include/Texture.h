#pragma once

#include <glad/gl.h>
#include <string>

namespace Forge {
class Texture {
public:
  Texture(const std::string &filePath);
  ~Texture();

  // Non-copyable because OpenGL texture handles have unique ownership.
  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;

  // Move constructor and assignment operator for efficient resource management.
  Texture(Texture &&other) noexcept;
  Texture &operator=(Texture &&other) noexcept;

  void bind(GLuint slot = 0) const;
  void unbind() const;
  bool isValid() const { return textureID != 0; }
  int getWidth() const { return width; };
  int getHeight() const { return height; };

private:
  GLuint textureID = 0;
  int width = 0;
  int height = 0;
};
} // namespace Forge
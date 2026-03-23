#pragma once

#include "Texture.h"
#include <string>

namespace Forge {

struct Rect {
  float x = 0, y = 0;
  float width = 0, height = 0;
};

class Sprite {
public:
  Sprite(const std::string &filePath);
  ~Sprite() = default;

  // Non-copyable because OpenGL texture handles have unique ownership.
  Sprite(const Sprite &) = delete;
  Sprite &operator=(const Sprite &) = delete;

  // Move constructor and assignment operator for efficient resource management.
  Sprite(Sprite &&other) noexcept;
  Sprite &operator=(Sprite &&other) noexcept;

  // Position
  void setPosition(float x, float y);
  int getX() const { return x; }
  int getY() const { return y; }

  // Size
  int getWidth() const { return texture.getWidth(); }
  int getHeight() const { return texture.getHeight(); }

  // UV Region - which part of the texture to use for this sprite (for sprite
  // sheets)
  void setUVRegion(float u, float v, float width, float height);
  const Rect &getUVRegion() const { return uvRegion; }

  const Texture &getTexture() const { return texture; }

private:
  Texture texture;
  float x = 0.0f;
  float y = 0.0f;
  Rect uvRegion = {0.0f, 0.0f, 1.0f, 1.0f}; // Full texture by default
};
} // namespace Forge
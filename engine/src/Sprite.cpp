#include "Sprite.h"

namespace Forge {
Sprite::Sprite(const std::string &filePath) : texture(filePath) {

}

Sprite::Sprite(Sprite &&other) noexcept
    : texture(std::move(other.texture)), x(other.x), y(other.y),
      uvRegion(other.uvRegion) {
  other.x = 0.0f;
  other.y = 0.0f;
  other.uvRegion = {0.0f, 0.0f, 1.0f, 1.0f};
}

Sprite &Sprite::operator=(Sprite &&other) noexcept {
  if (this != &other) {
    return *this;
  }
  texture = std::move(other.texture);
  x = other.x;
  y = other.y;
  uvRegion = other.uvRegion;
  other.x = 0.0f;
  other.y = 0.0f;
  other.uvRegion = {0.0f, 0.0f,
                      1.0f, 1.0f};
  return *this;
}
  void Sprite::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
  }

  void Sprite::setUVRegion(float u, float v, float width, float height) {
    uvRegion.x = u;
    uvRegion.y = v;
    uvRegion.width = width;
    uvRegion.height = height;
  }
} // namespace Forge
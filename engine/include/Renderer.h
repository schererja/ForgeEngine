#pragma once

#include "Shader.h"
#include "Texture.h"
#include <GL/glew.h>
#include <memory>

namespace Forge {

// Minimal renderer responsible for clearing and drawing a test primitive.
class Renderer {
public:
  Renderer();
  ~Renderer();

  // Non-copyable because OpenGL object handles have unique ownership.
  Renderer(const Renderer &) = delete;
  Renderer &operator=(const Renderer &) = delete;

  // Rendering API
  void setClearColor(float r, float g, float b, float a = 1.0f);
  void clear();
  void drawTestTriangle();
  void drawTestQuad(const Texture &texture);

private:
  GLuint vertexArray = 0;
  GLuint vertexBuffer = 0;
  std::unique_ptr<Shader> shader;

  // Allocates GPU buffers and shader state for the built-in triangle test.
  void setupTestTriangle();
};
} // namespace Forge
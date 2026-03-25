#pragma once

#include <glad/gl.h>

#include <glm/glm.hpp>
#include <memory>

#include "Camera.h"
#include "Shader.h"
#include "Sprite.h"
#include "Texture.h"

namespace Forge {

// Minimal renderer responsible for clearing and drawing a test primitive.
class Renderer {
   public:
    Renderer(int screenWidth, int screenHeight);
    ~Renderer();

    // Non-copyable because OpenGL object handles have unique ownership.
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    // Rendering API
    void setClearColor(float r, float g, float b, float a = 1.0f);
    void clear();
    void drawSprite(const Sprite& sprite);
    void setCamera(const Camera& camera);

   private:
    GLuint vertexArray = 0;
    GLuint vertexBuffer = 0;
    std::unique_ptr<Shader> shader;
    glm::mat4 viewProjection;

    // Allocates GPU buffers and shader state for the built-in triangle test.
    void setupQuad();
};
}  // namespace Forge
#include "Renderer.h"
#include <iostream>

namespace Forge {

// Minimal GLSL shaders embedded as strings for bootstrapping.

static const std::string VERTEX_SHADER = R"(
    #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord; // Using color attribute as "texture coordinate" for testing
        out vec2 texCoord;
        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            texCoord = aTexCoord; // Pass color as "texture coordinate" to fragment shader
        }
    )";

static const std::string FRAGMENT_SHADER = R"(
        #version 330 core
        in vec2 texCoord;
        out vec4 outColor;
        uniform sampler2D uTexture; // Not used in this test shader, but reserved for future use
        void main() {
            outColor = texture(uTexture, texCoord); // Sample the "texture" using the passed color as coordinates
        }
    )";
Renderer::Renderer() {
  setClearColor(0.117f, 0.117f, 0.117f, 1.0f);
  setupTestTriangle();
}

Renderer::~Renderer() {
  if (vertexBuffer) {
    glDeleteBuffers(1, &vertexBuffer);
  }
  if (vertexArray) {
    glDeleteVertexArrays(1, &vertexArray);
  }
}

void Renderer::setClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Renderer::setupTestTriangle() {
  // Interleaved position/color vertex data for a single triangle.
  float vertices[] = {
      // position       // UV
      -0.5f, 0.5f,  0.0f, 1.0f, // top left
      -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
      0.5f,  -0.5f, 1.0f, 0.0f, // bottom right

      -0.5f, 0.5f,  0.0f, 1.0f, // top left
      0.5f,  -0.5f, 1.0f, 0.0f, // bottom right
      0.5f,  0.5f,  1.0f, 1.0f  // top right
  };

  glGenVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  shader = std::make_unique<Shader>(VERTEX_SHADER, FRAGMENT_SHADER);
  if (!shader->isValid()) {
    std::cerr << "[Forge] Failed to create shader program." << std::endl;
  }
}

void Renderer::drawTestTriangle() {
  if (!shader || !shader->isValid()) {
    std::cerr << "[Forge] Cannot draw triangle: Shader program is not valid."
              << std::endl;
    return;
  }

  shader->bind();
  glBindVertexArray(vertexArray);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
  shader->unbind();
}

void Renderer::drawTestQuad(const Texture &texture) {
  if (!shader || !shader->isValid()) {
    std::cerr << "[Forge] Cannot draw quad: Shader program is not valid."
              << std::endl;
    return;
  }
  shader->bind();
  texture.bind(0);
  glBindVertexArray(vertexArray);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
  texture.unbind();
  shader->unbind();
}
} // namespace Forge
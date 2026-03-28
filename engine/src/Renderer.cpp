#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace Forge {

// Minimal GLSL shaders embedded as strings for bootstrapping.

Renderer::Renderer(int screenWidth, int screenHeight) {
    glClearColor(0.117f, 0.117f, 0.117f, 1.0f);

    // Orthographic projection with (0,0) at top-left and (screenWidth, screenHeight) at
    // bottom-right
    viewProjection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
    setupQuad();
}

Renderer::~Renderer() {
    if (vertexBuffer) {
        glDeleteBuffers(1, &vertexBuffer);
    }
    if (vertexArray) {
        glDeleteVertexArrays(1, &vertexArray);
    }
}

void Renderer::setClearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }

void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Renderer::setupQuad() {
    // Unit Quad -position and UV
    // Actual position/size set per draw call via vertex data
    float vertices[] = {
        // position       // UV
        0.0f, 0.0f, 0.0f, 0.0f,  // top left
        0.0f, 1.0f, 0.0f, 1.0f,  // bottom left
        1.0f, 1.0f, 1.0f, 1.0f,  // bottom right

        0.0f, 0.0f, 0.0f, 0.0f,  // top left
        1.0f, 1.0f, 1.0f, 1.0f,  // bottom right
        1.0f, 0.0f, 1.0f, 0.0f   // top right
    };

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    shader = std::make_unique<Shader>("game/assets/shaders/sprite.vert",
                                      "game/assets/shaders/sprite.frag");
    if (!shader->isValid()) {
        std::cerr << "[Forge] Failed to create shader program." << std::endl;
    }
}
// void Renderer::drawSprite(const Sprite& sprite) {
//     if (!shader || !shader->isValid()) {
//         std::cerr << "[Forge] Cannot draw sprite: shader program is not valid." << std::endl;
//         return;
//     }
//     const Rect& uv = sprite.getUVRegion();
//     float x = sprite.getX();
//     float y = sprite.getY();
//     float width = (float)sprite.getWidth();
//     float height = (float)sprite.getHeight();

//     // build quad vertex data with position and UVs based on sprite properties
//     float vertices[] = {
//         // position       // UV
//         x,
//         y,
//         uv.x,
//         uv.y,  // top left
//         x,
//         y + height,
//         uv.x,
//         uv.y + uv.height,  // bottom
//         x + width,
//         y + height,
//         uv.x + uv.width,
//         uv.y + uv.height,  // bottom right
//         x,
//         y,
//         uv.x,
//         uv.y,  // top left
//         x + width,
//         y + height,
//         uv.x + uv.width,
//         uv.y + uv.height,  // bottom right
//         x + width,
//         y,
//         uv.x + uv.width,
//         uv.y  // top right
//     };

//     shader->bind();

//     // Upload projection Matrix to shader

//     GLint projLoc = glGetUniformLocation(shader->getProgramID(), "uProjection");
//     glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(viewProjection));

//     // upload updated vertices to GPU
//     glBindVertexArray(vertexArray);
//     glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//     glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

//     sprite.getTexture().bind(0);
//     glDrawArrays(GL_TRIANGLES, 0, 6);

//     sprite.getTexture().unbind();
//     glBindVertexArray(0);
//     shader->unbind();
// }

void Renderer::setCamera(const Camera& camera) {
    // shader->bind();
    // GLint projLoc = glGetUniformLocation(shader->getProgramID(), "uProjection");
    // glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewProjection()));
    // shader->unbind();
    viewProjection = camera.getViewProjection();
}

void Renderer::drawEntities(EntityManager& entityManager, AssetManager& assetManager) {
    entityManager.forEach<TransformComponent, SpriteComponent>(
        [this, &assetManager](EntityID id, TransformComponent& transform, SpriteComponent& sprite) {
            Texture* texture = assetManager.getTexture(sprite.texturePath);
            if (texture) {
                drawTexture(texture, transform.x, transform.y, sprite.width, sprite.height);
            }
        });
}

void Renderer::drawTexture(Texture* texture, float x, float y, float width, float height) {
    if (!shader || !shader->isValid()) {
        return;
    }
    float vertices[] = {
        x,         y,          0.0f,
        0.0f,  // top left
        x,         y + height, 0.0f, 1.0f, x + width, y + height, 1.0f,
        1.0f,  // bottom right
        x,         y,          0.0f,
        0.0f,  // top left
        x + width, y + height, 1.0f,
        1.0f,  // bottom right
        x + width, y,          1.0f,
        0.0f  // top right
    };
    shader->bind();

    GLint projLoc = glGetUniformLocation(shader->getProgramID(), "uProjection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(viewProjection));

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    texture->bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    texture->unbind();

    glBindVertexArray(0);
    shader->unbind();
}

void Renderer::drawTilemap(const Tilemap& tilemap, AssetManager& assets) {
    const Tileset& tileset = tilemap.getTileset();
    Texture* texture = assets.getTexture(tileset.texturePath);
    if (!texture) {
        std::cerr << "[FORGE] Cannot draw tilemap: tileset texture not found: "
                  << tileset.texturePath << std::endl;
        return;
    }
    if (!shader || !shader->isValid()) {
        std::cerr << "[FORGE] Cannot draw tilemap: shader program is not valid." << std::endl;
        return;
    }

    shader->bind();
    GLint projLoc = glGetUniformLocation(shader->getProgramID(), "uProjection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(viewProjection));

    int tileWidth = tilemap.getTileWidth();
    int tileHeight = tilemap.getTileHeight();

    // UV size of one tile in the tileset
    float uvTileWidth = 1.0f / tileset.columns;
    float uvTileHeight = 1.0f / tileset.rows;

    // Build ALL tile vertices in one go for simplicity. For large maps, consider batching or
    // culling.
    std::vector<float> vertices;
    vertices.reserve(tilemap.getMapWidth() * tilemap.getMapHeight() * 6 *
                     4);  // 6 vertices per tile, 4 floats per vertex
    for (int y = 0; y < tilemap.getMapHeight(); y++) {
        for (int x = 0; x < tilemap.getMapWidth(); x++) {
            const Tile& tile = tilemap.getTile(x, y);
            if (!tile.visible) {
                continue;  // Skip invisible tiles
            }

            // World position of the tile
            float worldX = x * tileWidth;
            float worldY = y * tileHeight;

            // which row/col in the tileset texture
            int col = tile.tileID % tileset.columns;
            int row = tile.tileID / tileset.columns;

            // UV coordinates for the tile
            float u0 = col * uvTileWidth;
            float v0 = row * uvTileHeight;
            float u1 = u0 + uvTileWidth;
            float v1 = v0 + uvTileHeight;

            // Triangles for this tile
            vertices.insert(vertices.end(), {

                                                worldX,
                                                worldY,
                                                u0,
                                                v0,  // top left
                                                worldX,
                                                worldY + tileHeight,
                                                u0,
                                                v1,  // bottom left
                                                worldX + tileWidth,
                                                worldY + tileHeight,
                                                u1,
                                                v1,  // bottom right

                                            });

            // Triangle 2
            vertices.insert(vertices.end(), {

                                                worldX, worldY, u0,
                                                v0,  // top left
                                                worldX + tileWidth, worldY + tileHeight, u1,
                                                v1,  // bottom right
                                                worldX + tileWidth, worldY, u1,
                                                v0  // top right
                                            });
            if (vertices.empty()) {
                continue;  // Skip if no vertices to draw
            }
            shader->bind();
            GLint projLoc = glGetUniformLocation(shader->getProgramID(), "uProjection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(viewProjection));

            // Upload all tile vertices to GPU
            glBindVertexArray(vertexArray);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(),
                         GL_DYNAMIC_DRAW);
            texture->bind(0);

            // One draw call for the entire tilemap. For large maps, consider chunking and culling.
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(vertices.size() / 4));
            texture->unbind();
            glBindVertexArray(0);
            shader->unbind();
        }
    }
}
}  // namespace Forge
#pragma once

#include <glm/glm.hpp>

namespace Forge {

class Camera {
   public:
    Camera(int screenWidth, int screenHeight);

    // Absoulate position in world space
    void setPosition(float x, float y);

    // Move the camera relate to the current position
    void move(float deltaX, float deltaY);

    // Get the combined view-projection matrix for transforming world coordinates to screen space
    const glm::mat4& getViewProjection() const { return viewProjection; }

    float getX() const { return x; }
    float getY() const { return y; }

   private:
    float x = 0.0f;
    float y = 0.0f;

    int screenWidth;
    int screenHeight;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 viewProjection;

    void recalculateViewProjection();
};
}  // namespace Forge
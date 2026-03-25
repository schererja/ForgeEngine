#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Forge {
Camera::Camera(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight) {
    projection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
    recalculateViewProjection();
}

void Camera::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
    recalculateViewProjection();
}

void Camera::move(float deltaX, float deltaY) {
    x += deltaX;
    y += deltaY;
    recalculateViewProjection();
}

void Camera::recalculateViewProjection() {
    // In this simple 2D camera, the view matrix is just a translation based on the camera's
    // position. Moving Camera to the right (positive x) should move the world to the left, so we
    // translate by -x.
    view = glm::translate(glm::mat4(1.0f), glm::vec3(-x, -y, 0.0f));
    viewProjection = projection * view;
}

}  // namespace Forge
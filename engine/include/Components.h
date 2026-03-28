#pragma once

#include <string>

namespace Forge {

// Position and size in the world space
struct TransformComponent {
    float x = 0.0f;
    float y = 0.0f;
    float rotation = 0.0f;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
};

// Rendering
struct SpriteComponent {
    std::string texturePath;
    float width = 32.0f;
    float height = 32.0f;
    int layer = 0;  // For rendering order
};

// Health - generic enough for both player and enemies
struct HealthComponent {
    float currentHealth = 100.0f;
    float maxHealth = 100.0f;

    bool isAlive() const { return currentHealth > 0.0f; }

    float getPercentage() const {
        return (maxHealth > 0.0f) ? (currentHealth / maxHealth) * 100.0f : 0.0f;
    }

    void takeDamage(float amount) {
        currentHealth -= amount;
        if (currentHealth < 0.0f) {
            currentHealth = 0.0f;
        }
    }

    void heal(float amount) {
        currentHealth += amount;
        if (currentHealth > maxHealth) {
            currentHealth = maxHealth;
        }
    }
};

// Marks an entity as a player
struct PlayerComponent {
    float moveSpeed = 200.0f;  // Movement speed in units per second
};

struct AIComponent {
    bool active = true;         // Whether the AI is active or not
    float aggroRange = 150.0f;  // Distance at which the AI will start chasing the player
};

// Name tag for debugging purposes and potential UI display
struct NameComponent {
    std::string name;
};

// axis-aligned bounding box for simple collision detection
struct CollisionComponent {
    float offsetX = 0.0f;  // Offset from the entity's position
    float offsetY = 0.0f;
    float width = 32.0f;  // Size of the collision box
    float height = 32.0f;
    bool solid = true;  // Whether this entity should block movement
};
}  // namespace Forge
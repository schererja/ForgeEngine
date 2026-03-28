#pragma once

#include <glm/glm.hpp>

#include "Components.h"
#include "Tilemap.h"

namespace Forge {
struct AABB {
    float x, y;
    float width, height;
    float left() const { return x; }
    float right() const { return x + width; }
    float top() const { return y; }
    float bottom() const { return y + height; }

    bool overlaps(const AABB& other) const {
        return !(right() <= other.left() || left() >= other.right() || bottom() <= other.top() ||
                 top() >= other.bottom());
    }
};

class CollisionSystem {
   public:
    // Resolve movement against tilemap solid tiles and solid entities. Returns the adjusted
    // position after collision resolution.
    static glm::vec2 resolveMapCollision(float x, float y, float width, float height, float newX,
                                         float newY, const Tilemap& tilemap);
    // Check if an AABB collides with any solid entities. Returns true if a collision is detected.
    static bool overlapSolidTile(const AABB& box, const Tilemap& tilemap);
    static AABB getAABB(const TransformComponent& transform, const CollisionComponent& collision);
};
}  // namespace Forge
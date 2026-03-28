#include "CollisionSystem.h"

#include <algorithm>
#include <cmath>

namespace Forge {
AABB CollisionSystem::getAABB(const TransformComponent& transform,
                              const CollisionComponent& collision) {
    return AABB{transform.x + collision.offsetX, transform.y + collision.offsetY, collision.width,
                collision.height};
};

bool CollisionSystem::overlapSolidTile(const AABB& box, const Tilemap& tilemap) {
    int tileWidth = tilemap.getTileWidth();
    int tileHeight = tilemap.getTileHeight();
    // Check which tiles the AABB overlaps with
    int leftTile = static_cast<int>(std::floor(box.left() / tileWidth));
    int rightTile = static_cast<int>(std::floor(box.right() / tileWidth));
    int topTile = static_cast<int>(std::floor(box.top() / tileHeight));
    int bottomTile = static_cast<int>(std::floor(box.bottom() / tileHeight));

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            if (tilemap.isInBounds(x, y)) {
                const Tile& tile = tilemap.getTile(x, y);
                if (tile.solid) {
                    return true;
                }
            }
        }
    }
    return false;
}

glm::vec2 CollisionSystem::resolveMapCollision(float x, float y, float width, float height,
                                               float newX, float newY, const Tilemap& tilemap) {
    // Try X movement first
    AABB boxX = {newX, y, width, height};
    if (overlapSolidTile(boxX, tilemap)) {
        newX = x;  // block horizontal movement
    }

    // Try Y movement independently
    AABB boxY = {newX, newY, width, height};
    if (overlapSolidTile(boxY, tilemap)) {
        newY = y;  // block vertical movement
    }
    return {newX, newY};
}

}  // namespace Forge
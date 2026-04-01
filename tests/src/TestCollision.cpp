#include <CollisionSystem.h>
#include <Tilemap.h>

#include <catch2/catch_test_macros.hpp>

using namespace Forge;

TEST_CASE("AABB overlap detection", "[collision]") {
    SECTION("Overlapping boxes return true") {
        AABB a = {0.0f, 0.0f, 32.0f, 32.0f};
        AABB b = {16.0f, 16.0f, 32.0f, 32.0f};
        REQUIRE(a.overlaps(b));
    }

    SECTION("Non-overlapping boxes return false") {
        AABB a = {0.0f, 0.0f, 32.0f, 32.0f};
        AABB b = {64.0f, 0.0f, 32.0f, 32.0f};
        REQUIRE_FALSE(a.overlaps(b));
    }

    SECTION("Adjacent boxes do not overlap") {
        AABB a = {0.0f, 0.0f, 32.0f, 32.0f};
        AABB b = {32.0f, 0.0f, 32.0f, 32.0f};
        REQUIRE_FALSE(a.overlaps(b));
    }
}

TEST_CASE("Tilemap solid tile detection", "[collision][tilemap]") {
    Tilemap map(10, 10, 32, 32);

    SECTION("Non-solid tile does not block") {
        map.setTile(5, 5, 0, false);
        AABB box = {5 * 32.0f, 5 * 32.0f, 32.0f, 32.0f};
        REQUIRE_FALSE(CollisionSystem::overlapSolidTile(box, map));
    }

    SECTION("Solid tile blocks") {
        map.setTile(5, 5, 0, true);
        AABB box = {5 * 32.0f, 5 * 32.0f, 32.0f, 32.0f};
        REQUIRE(CollisionSystem::overlapSolidTile(box, map));
    }

    SECTION("Out of bounds does not crash") {
        AABB box = {-100.0f, -100.0f, 32.0f, 32.0f};
        REQUIRE_FALSE(CollisionSystem::overlapSolidTile(box, map));
    }
}

TEST_CASE("Map collision resolution", "[collision]") {
    Tilemap map(10, 10, 32, 32);
    map.setTile(5, 5, 0, true);  // solid tile at (5,5)

    SECTION("No collision allows movement") {
        glm::vec2 result = CollisionSystem::resolveMapCollision(
            100.0f, 100.0f, 32.0f, 32.0f, 110.0f, 100.0f, map);
        REQUIRE(result.x == 110.0f);
        REQUIRE(result.y == 100.0f);
    }

    SECTION("Horizontal collision blocks X movement") {
        // Start just left of solid tile, try to move into it
        float startX = 5 * 32.0f - 32.0f;
        glm::vec2 result = CollisionSystem::resolveMapCollision(
            startX, 5 * 32.0f, 32.0f, 32.0f, startX + 10.0f, 5 * 32.0f, map);
        REQUIRE(result.x == startX);     // X blocked
        REQUIRE(result.y == 5 * 32.0f);  // Y unchanged
    }
}
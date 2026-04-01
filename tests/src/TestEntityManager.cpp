#include <Components.h>
#include <EntityManager.h>

#include <catch2/catch_test_macros.hpp>

using namespace Forge;

TEST_CASE("EntityManager creates valid entities", "[entity]") {
    EntityManager em;

    SECTION("Created entity is valid") {
        EntityID id = em.createEntity();
        REQUIRE(id != NULL_ENTITY);
        REQUIRE(em.isEntityAlive(id));
    }

    SECTION("Multiple entities have unique IDs") {
        EntityID id1 = em.createEntity();
        EntityID id2 = em.createEntity();
        EntityID id3 = em.createEntity();
        REQUIRE(id1 != id2);
        REQUIRE(id2 != id3);
        REQUIRE(id1 != id3);
    }

    SECTION("Entity count is correct") {
        em.createEntity();
        em.createEntity();
        em.createEntity();
        REQUIRE(em.getEntityCount() == 3);
    }
}

TEST_CASE("EntityManager destroys entities correctly", "[entity]") {
    EntityManager em;

    SECTION("Destroyed entity is no longer alive") {
        EntityID id = em.createEntity();
        em.destroyEntity(id);
        REQUIRE_FALSE(em.isEntityAlive(id));
    }

    SECTION("Entity count decreases on destroy") {
        EntityID id1 = em.createEntity();
        EntityID id2 = em.createEntity();
        em.destroyEntity(id1);
        REQUIRE(em.getEntityCount() == 1);
    }

    SECTION("Destroyed entity ID gets recycled") {
        EntityID id1 = em.createEntity();
        em.destroyEntity(id1);
        EntityID id2 = em.createEntity();
        REQUIRE(id1 == id2);  // recycled
    }
}

TEST_CASE("EntityManager handles components correctly", "[entity][component]") {
    EntityManager em;

    SECTION("Can add and retrieve a component") {
        EntityID id = em.createEntity();
        em.addComponent<TransformComponent>(id, {100.0f, 200.0f});

        auto* transform = em.getComponent<TransformComponent>(id);
        REQUIRE(transform != nullptr);
        REQUIRE(transform->x == 100.0f);
        REQUIRE(transform->y == 200.0f);
    }

    SECTION("hasComponent returns correct values") {
        EntityID id = em.createEntity();
        REQUIRE_FALSE(em.hasComponent<TransformComponent>(id));

        em.addComponent<TransformComponent>(id, {0.0f, 0.0f});
        REQUIRE(em.hasComponent<TransformComponent>(id));
    }

    SECTION("Removing component works correctly") {
        EntityID id = em.createEntity();
        em.addComponent<TransformComponent>(id, {0.0f, 0.0f});
        em.removeComponent<TransformComponent>(id);
        REQUIRE_FALSE(em.hasComponent<TransformComponent>(id));
    }

    SECTION("getComponent returns nullptr for missing component") {
        EntityID id = em.createEntity();
        auto* transform = em.getComponent<TransformComponent>(id);
        REQUIRE(transform == nullptr);
    }

    SECTION("Destroying entity removes all components") {
        EntityID id = em.createEntity();
        em.addComponent<TransformComponent>(id, {0.0f, 0.0f});
        em.addComponent<HealthComponent>(id, {100.0f, 100.0f});
        em.destroyEntity(id);

        EntityID newId = em.createEntity();  // recycled ID
        REQUIRE_FALSE(em.hasComponent<TransformComponent>(newId));
        REQUIRE_FALSE(em.hasComponent<HealthComponent>(newId));
    }
}
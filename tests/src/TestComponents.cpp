#include <Components.h>

#include <catch2/catch_test_macros.hpp>

using namespace Forge;

TEST_CASE("HealthComponent works correctly", "[component]") {
    HealthComponent health;
    health.currentHealth = 100.0f;
    health.maxHealth = 100.0f;

    SECTION("isAlive returns true when health > 0") {
        REQUIRE(health.isAlive());
    }

    SECTION("takeDamage reduces health") {
        health.takeDamage(30.0f);
        REQUIRE(health.currentHealth == 70.0f);
    }

    SECTION("takeDamage does not go below zero") {
        health.takeDamage(200.0f);
        REQUIRE(health.currentHealth == 0.0f);
    }

    SECTION("isAlive returns false when health is zero") {
        health.takeDamage(100.0f);
        REQUIRE_FALSE(health.isAlive());
    }

    SECTION("heal increases health") {
        health.takeDamage(50.0f);
        health.heal(20.0f);
        REQUIRE(health.currentHealth == 70.0f);
    }

    SECTION("heal does not exceed maximum") {
        health.heal(50.0f);
        REQUIRE(health.currentHealth == 100.0f);
    }

    SECTION("getPercent returns correct value") {
        health.takeDamage(50.0f);
        REQUIRE(health.getPercentage() == 0.5f);
    }
}

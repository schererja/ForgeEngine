#include "OverworldScene.h"

#include <CollisionSystem.h>
#include <Components.h>
#include <Input.h>

#include <iostream>

void OverworldScene::onEnter(Forge::Engine& engine) {
    std::cout << "[OverworldScene] Entering scene." << std::endl;
    // Create a simple tilemap
    tilemap = Forge::Tilemap(20, 15, 32, 32);
    Forge::Tileset tileset;

    tileset.texturePath = "../game/assets/tileset.png";
    tileset.tileWidth = 48;
    tileset.tileHeight = 48;
    tileset.columns = 3;
    tileset.rows = 3;
    tilemap.setTileset(tileset);

    tilemap.fill(1, false);  // Fill with non-solid tiles

    // Stone border
    for (int x = 0; x < 20; x++) {
        tilemap.setTile(x, 0, 0, true);
        tilemap.setTile(x, 14, 0, true);
    }

    for (int y = 0; y < 15; y++) {
        tilemap.setTile(0, y, 0, true);
        tilemap.setTile(19, y, 0, true);
    }

    // Water obstacle
    tilemap.setTile(5, 5, 2, true);
    tilemap.setTile(5, 6, 2, true);
    tilemap.setTile(5, 7, 2, true);
    tilemap.setTile(6, 5, 2, true);
    tilemap.setTile(7, 5, 2, true);

    // Create player entity
    playerEntity = engine.getEntityManager().createEntity();
    engine.getEntityManager().addComponent<Forge::TransformComponent>(
        playerEntity, {64.0f, 64.0f});
    engine.getEntityManager().addComponent<Forge::SpriteComponent>(
        playerEntity, {"../game/assets/sprite.png", 32.0f, 32.0f, 1});
    engine.getEntityManager().addComponent<Forge::PlayerComponent>(playerEntity,
                                                                   {200.0f});
    engine.getEntityManager().addComponent<Forge::NameComponent>(playerEntity,
                                                                 {"Player"});
}

void OverworldScene::onExit(Forge::Engine& engine) {
    std::cout << "[OverworldScene] Exiting scene." << std::endl;
    engine.getEntityManager().destroyEntity(playerEntity);
    playerEntity = Forge::NULL_ENTITY;
}

void OverworldScene::update(Forge::Engine& engine, float deltaTime) {
    auto* transform =
        engine.getEntityManager().getComponent<Forge::TransformComponent>(
            playerEntity);
    auto* playerComp =
        engine.getEntityManager().getComponent<Forge::PlayerComponent>(
            playerEntity);
    auto* input = &engine.getInput();
    if (transform && playerComp && input) {
        float speed = playerComp->moveSpeed * deltaTime;
        float newX = transform->x;
        float newY = transform->y;

        if (input->isKeyHeldDown(Forge::Key::RIGHT) ||
            input->isKeyHeldDown(Forge::Key::D))
            newX += speed;
        if (input->isKeyHeldDown(Forge::Key::LEFT) ||
            input->isKeyHeldDown(Forge::Key::A))
            newX -= speed;
        if (input->isKeyHeldDown(Forge::Key::DOWN) ||
            input->isKeyHeldDown(Forge::Key::S))
            newY += speed;
        if (input->isKeyHeldDown(Forge::Key::UP) ||
            input->isKeyHeldDown(Forge::Key::W))
            newY -= speed;

        glm::vec2 resolved = Forge::CollisionSystem::resolveMapCollision(
            transform->x, transform->y, 32.0f, 32.0f, newX, newY, tilemap);

        transform->x = resolved.x;
        transform->y = resolved.y;

        engine.getCamera().setPosition(
            transform->x - engine.getWidth() / 2.0f,
            transform->y - engine.getHeight() / 2.0f);
    }
}

void OverworldScene::render(Forge::Engine& engine) {
    engine.getRenderer().drawTilemap(tilemap, engine.getAssetManager());
    engine.getRenderer().drawEntities(engine.getEntityManager(),
                                      engine.getAssetManager());
}
#include "OverworldScene.h"

#include <CollisionSystem.h>
#include <Components.h>
#include <Input.h>
#include <Log.h>
#include <MapLoader.h>

#include <iostream>

void OverworldScene::onEnter(Forge::Engine& engine) {
    FORGE_INFO("Entering OverworldScene.");
    std::string assetPath =
        engine.getConfig().getString("paths.assets", "../game/assets/");
    std::string audioPath =
        engine.getConfig().getString("paths.audio", assetPath + "audio/");
    std::string mapsPath =
        engine.getConfig().getString("paths.maps", assetPath + "maps/");
    // Load map from file
    Forge::MapData mapData =
        Forge::MapLoader::loadFromFile(mapsPath + "overworld.lua", assetPath);

    if (!mapData.valid) {
        FORGE_ERROR("Failed to load map data. Falling back to default map.");
        return;
    }

    tilemap = std::move(mapData.tilemap);

    // Spawn entities
    for (const auto& spawn : mapData.spawns) {
        if (spawn.type == "Player") {
            float w = spawn.width > 0 ? spawn.width : 32.0f;
            float h = spawn.height > 0 ? spawn.height : 32.0f;
            playerEntity = engine.getEntityManager().createEntity();
            engine.getEntityManager().addComponent<Forge::TransformComponent>(
                playerEntity, {spawn.x, spawn.y});
            engine.getEntityManager().addComponent<Forge::SpriteComponent>(
                playerEntity, {assetPath + "sprite.png", w, h, 0});
            engine.getEntityManager().addComponent<Forge::PlayerComponent>(
                playerEntity, {200.0f});
            engine.getEntityManager().addComponent<Forge::NameComponent>(
                playerEntity, {spawn.name});
        }
    }

    // Play background music
    engine.getAudio().playMusic(audioPath + "theme.mp3", 0.5f);
}
void OverworldScene::onExit(Forge::Engine& engine) {
    engine.getAudio().stopMusic();
    FORGE_INFO("Exiting OverworldScene and cleaning up resources.");
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
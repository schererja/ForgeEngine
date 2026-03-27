#include "ForgeEngine.h"

#include <iostream>

#include "Components.h"
#include "Sprite.h"

namespace Forge {

bool Engine::initialize(const EngineConfig& config) {
    window = new Window(config.windowTitle, config.windowWidth, config.windowHeight);
    renderer = new Renderer(config.windowWidth, config.windowHeight);
    input = new Input();

    if (!window->isOpen()) {
        std::cerr << "[FORGE] Failed to initialize window." << std::endl;
        return false;
    }

    std::cout << "[FORGE] Engine initialized successfully." << std::endl;
    return true;
}

void Engine::run() {
    if (!window || !renderer) {
        std::cerr << "[FORGE] Engine not initialized. Call initialize() before "
                     "run()."
                  << std::endl;
        return;
    }

    EntityID player = entityManager.createEntity();

    entityManager.addComponent<TransformComponent>(player, {100.0f, 100.0f});
    entityManager.addComponent<SpriteComponent>(player,
                                                {"../game/assets/sprite.png", 32.0f, 32.0f, 0});
    entityManager.addComponent<PlayerComponent>(player, {200.0f});
    entityManager.addComponent<NameComponent>(player, {"Player"});

    EntityID landmark = entityManager.createEntity();
    entityManager.addComponent<TransformComponent>(landmark, {400.0f, 300.0f});
    entityManager.addComponent<SpriteComponent>(landmark,
                                                {"../game/assets/test.png", 48.0f, 48.0f, 0});
    // Verify entity components
    std::cout << "[FORGE] Verifying components..." << std::endl;

    auto* transform = entityManager.getComponent<TransformComponent>(player);
    auto* sprite = entityManager.getComponent<SpriteComponent>(player);
    auto* playerC = entityManager.getComponent<PlayerComponent>(player);
    auto* name = entityManager.getComponent<NameComponent>(player);

    std::cout << "[FORGE] TransformComponent: "
              << (transform ? "OK x=" + std::to_string(transform->x) +
                                  " y=" + std::to_string(transform->y)
                            : "MISSING")
              << std::endl;

    std::cout << "[FORGE] SpriteComponent:   "
              << (sprite ? "OK path=" + sprite->texturePath : "MISSING") << std::endl;

    std::cout << "[FORGE] PlayerComponent:   "
              << (playerC ? "OK speed=" + std::to_string(playerC->moveSpeed) : "MISSING")
              << std::endl;

    std::cout << "[FORGE] NameComponent:     " << (name ? "OK name=" + name->name : "MISSING")
              << std::endl;

    std::cout << "[FORGE] Entity count: " << entityManager.getEntityCount() << std::endl;
    Camera camera(1280, 720);
    std::cout << "[FORGE] Starting main loop." << std::endl;

    // Basic game loop: process input, render frame, present.
    while (window->isOpen()) {
        window->pollEvents(input);

        // Move player entity based on input
        auto* transform = entityManager.getComponent<TransformComponent>(player);
        auto* playerComp = entityManager.getComponent<PlayerComponent>(player);
        if (transform && playerComp) {
            float moveSpeed = playerComp->moveSpeed * (1.0f / 60.0f);  // Assuming 60 FPS
            if (input->isKeyHeldDown(Key::RIGHT) || input->isKeyHeldDown(Key::D)) {
                transform->x += moveSpeed;
            }
            if (input->isKeyHeldDown(Key::LEFT) || input->isKeyHeldDown(Key::A)) {
                transform->x -= moveSpeed;
            }
            if (input->isKeyHeldDown(Key::UP) || input->isKeyHeldDown(Key::W)) {
                transform->y -= moveSpeed;
            }
            if (input->isKeyHeldDown(Key::DOWN) || input->isKeyHeldDown(Key::S)) {
                transform->y += moveSpeed;
            }
            // camera.setPosition(transform->x - 640, transform->y - 360);
        }

        renderer->setCamera(camera);
        renderer->clear();
        renderer->drawEntities(entityManager, assetManager);
        window->swapBuffers();
    }

    std::cout << "[FORGE] Main loop exited." << std::endl;
}

void Engine::shutdown() {
    assetManager.unloadAll();
    delete input;
    delete renderer;
    delete window;
    input = nullptr;
    renderer = nullptr;
    window = nullptr;

    std::cout << "[FORGE] Engine shutdown complete." << std::endl;
}
}  // namespace Forge
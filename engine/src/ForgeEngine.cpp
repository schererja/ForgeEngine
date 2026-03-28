#include "ForgeEngine.h"

#include <iostream>

#include "CollisionSystem.h"
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
    // Create a simple test tilemap for demonstration
    Tilemap tilemap(20, 15, 48, 48);
    // Set up a basic tileset (assuming a 256x256 texture with 48x48 tiles)
    Tileset tileset;
    tileset.texturePath = "../game/assets/tileset.png";
    tileset.tileWidth = 48;
    tileset.tileHeight = 48;
    tileset.columns = 3;  // 256/48 = 5 columns
    tileset.rows = 3;     // 256/48 = 5 rows
    tilemap.setTileset(tileset);
    // Fill the tilemap with tileID 0 (which corresponds to the single tile in the tileset)
    tilemap.fill(1, false);
    for (int x = 0; x < 20; x++) {
        tilemap.setTile(x, 0, 0, true);   // Make the top row solid
        tilemap.setTile(x, 14, 0, true);  // Make the bottom row solid
    }
    for (int y = 0; y < 15; y++) {
        tilemap.setTile(0, y, 0, true);   // Make the left column solid
        tilemap.setTile(19, y, 0, true);  // Make the right column solid
    }
    tilemap.setTile(5, 5, 2, true);
    tilemap.setTile(6, 5, 2, true);
    tilemap.setTile(5, 6, 2, true);
    tilemap.setTile(6, 6, 2, true);
    EntityID player = entityManager.createEntity();
#pragma region Component Setup
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
#pragma endregion
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
    // Delta time for movement calculations
    Uint64 lastTime = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();
    float deltaTime = 0.0f;
    float maxDelta = 0.05f;

    std::cout << "[FORGE] Starting main loop." << std::endl;
    float fpsAccumulator = 0.0f;
    int fpsFrameCount = 0;
    float displayFPS = 0.0f;
    // Basic game loop: process input, render frame, present.
    while (window->isOpen()) {
        // Calculate delta time
        Uint64 currentTime = SDL_GetPerformanceCounter();
        deltaTime = (float)(currentTime - lastTime) / (float)frequency;
        deltaTime = std::min(deltaTime, maxDelta);  // Clamp to avoid big jumps
        lastTime = currentTime;
        fpsAccumulator += deltaTime;
        fpsFrameCount++;

        if (fpsAccumulator >= 0.25f) {  // update display every quarter second
            displayFPS = fpsFrameCount / fpsAccumulator;
            fpsAccumulator = 0.0f;
            fpsFrameCount = 0;
        }

        std::string title = "ForgeEngine | FPS: " + std::to_string((int)displayFPS);
        SDL_SetWindowTitle(window->getSDLWindow(), title.c_str());
        window->pollEvents(input);

        // Move player entity based on input
        auto* transform = entityManager.getComponent<TransformComponent>(player);
        auto* playerComp = entityManager.getComponent<PlayerComponent>(player);
        if (transform && playerComp) {
            // Scale by delta time for consistent movement
            float moveSpeed =
                playerComp->moveSpeed * deltaTime;  // Scale by delta time for consistent movement
            float newX = transform->x;
            float newY = transform->y;
            if (input->isKeyHeldDown(Key::RIGHT) || input->isKeyHeldDown(Key::D)) {
                newX += moveSpeed;
            }
            if (input->isKeyHeldDown(Key::LEFT) || input->isKeyHeldDown(Key::A)) {
                newX -= moveSpeed;
            }
            if (input->isKeyHeldDown(Key::UP) || input->isKeyHeldDown(Key::W)) {
                newY -= moveSpeed;
            }
            if (input->isKeyHeldDown(Key::DOWN) || input->isKeyHeldDown(Key::S)) {
                newY += moveSpeed;
            }
            // Resolve collisions with the tilemap
            glm::vec2 resolvedPos = CollisionSystem::resolveMapCollision(
                transform->x, transform->y, 32.0f, 32.0f, newX, newY, tilemap);
            transform->x = resolvedPos.x;
            transform->y = resolvedPos.y;
            camera.setPosition(transform->x - 640, transform->y - 360);
        }
        // std::cout << "dt: " << deltaTime << " fps: " << (1.0f / deltaTime) << std::endl;
        renderer->setCamera(camera);
        renderer->clear();
        renderer->drawTilemap(tilemap, assetManager);
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
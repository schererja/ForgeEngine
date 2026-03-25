#include "ForgeEngine.h"

#include <iostream>

#include "sprite.h"

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

    Sprite testSprite("../game/assets/sprite.png");
    testSprite.setPosition(100.0f, 100.0f);

    Camera camera(1280, 720);
    float cameraSpeed = 3.0f;
    std::cout << "[FORGE] Starting main loop." << std::endl;

    // Basic game loop: process input, render frame, present.
    while (window->isOpen()) {
        window->pollEvents(input);
        if(input->isKeyHeldDown(Key::RIGHT) || input->isKeyHeldDown(Key::D)) {
            camera.move(cameraSpeed, 0.0f);
        }
        if(input->isKeyHeldDown(Key::LEFT) || input->isKeyHeldDown(Key::A)) {
            camera.move(-cameraSpeed, 0.0f);
        }
        if(input->isKeyHeldDown(Key::UP) || input->isKeyHeldDown(Key::W)) {
            camera.move(0.0f, -cameraSpeed);
        }
        if(input->isKeyHeldDown(Key::DOWN) || input->isKeyHeldDown(Key::S)) {
            camera.move(0.0f, cameraSpeed);
        }

        renderer->setCamera(camera);
        renderer->clear();
        renderer->drawSprite(testSprite);
        window->swapBuffers();
    }

    std::cout << "[FORGE] Main loop exited." << std::endl;
}

void Engine::shutdown() {
    delete renderer;
    delete window;
    renderer = nullptr;
    window = nullptr;
    std::cout << "[FORGE] Engine shutdown complete." << std::endl;
}
}  // namespace Forge
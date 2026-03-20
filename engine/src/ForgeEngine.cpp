#include "ForgeEngine.h"
#include <iostream>

namespace Forge {

    bool Engine::initialize(const EngineConfig& config) {
        window = new Window(config.windowTitle, config.windowWidth, config.windowHeight);
        renderer = new Renderer();
        if (!window->isOpen()) {
            std::cerr << "[FORGE] Failed to initialize window." << std::endl;
            return false;
        }
        std::cout << "[FORGE] Engine initialized successfully." << std::endl;
        return true;
    }

    void Engine::run() {
        if (!window || !renderer) {
            std::cerr << "[FORGE] Engine not initialized. Call initialize() before run()." << std::endl;
            return;
        }

        std::cout << "[FORGE] Starting main loop." << std::endl;

        while (window->isOpen()) {
            window->pollEvents();
            renderer->clear();
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
}
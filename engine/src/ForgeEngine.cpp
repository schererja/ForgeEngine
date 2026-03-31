#include "ForgeEngine.h"

#include <iostream>

#include "CollisionSystem.h"
#include "Components.h"
#include "Sprite.h"
namespace Forge {

bool Engine::initialize(const EngineConfig& config) {
    width = config.windowWidth;
    height = config.windowHeight;
    window = new Window(config.windowTitle, width, height);
    renderer = new Renderer(width, height);
    input = new Input();
    camera = Camera(width, height);

    if (!window->isOpen()) {
        std::cerr << "[FORGE] Failed to initialize window." << std::endl;
        return false;
    }
    if (!audioSystem.initialize()) {
        std::cerr << "[FORGE] Failed to initialize audio system." << std::endl;
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
    if (sceneManager.sceneCount() == 0) {
        std::cerr << "[FORGE] No active scenes. Please push a scene before "
                     "running the engine."
                  << std::endl;
        return;
    }

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

        std::string title =
            "ForgeEngine | FPS: " + std::to_string((int)displayFPS);
        SDL_SetWindowTitle(window->getSDLWindow(), title.c_str());

        window->pollEvents(input);

        sceneManager.update(*this, deltaTime);
        renderer->setCamera(camera);
        renderer->clear();
        sceneManager.render(*this);
        window->swapBuffers();

        if (deltaTime >
            0.033f) {  // Warn if frame took longer than ~33ms (30 FPS)
            std::cerr << "[FORGE] Warning: Slow frame detected. Delta time: "
                      << deltaTime << " seconds." << std::endl;
        }

        if (sceneManager.sceneCount() == 0) {
            std::cout
                << "[FORGE] No active scenes remaining. Exiting main loop."
                << std::endl;
            break;
        }
    }

    std::cout << "[FORGE] Main loop exited." << std::endl;
}

void Engine::shutdown() {
    audioSystem.shutdown();
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
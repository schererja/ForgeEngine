#include "ForgeEngine.h"

#include <iostream>

#include "CollisionSystem.h"
#include "Components.h"
#include "Log.h"
#include "Sprite.h"
namespace Forge {

bool Engine::initialize(const EngineConfig& config) {
    // Logging has to be first
    Log::init();

    width = config.windowWidth;
    height = config.windowHeight;
    window = new Window(config.windowTitle, width, height);
    renderer = new Renderer(width, height);
    input = new Input();
    camera = Camera(width, height);

    if (!window->isOpen()) {
        FORGE_ERROR("Failed to initialize window.");
        return false;
    }
    if (!audioSystem.initialize()) {
        FORGE_ERROR("Failed to initialize audio system.");
        return false;
    }
    FORGE_INFO("Engine initialized successfully.");
    return true;
}

void Engine::run() {
    if (!window || !renderer) {
        FORGE_ERROR(
            "Engine not properly initialized. Call initialize() before run().");

        return;
    }
    if (sceneManager.sceneCount() == 0) {
        FORGE_WARN(
            "No active scenes. Please push a scene before running the engine.");

        return;
    }

    // Delta time for movement calculations
    Uint64 lastTime = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();
    float deltaTime = 0.0f;
    float maxDelta = 0.05f;
    FORGE_INFO("Entering main loop.");
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
            FORGE_WARN("Slow frame detected. Delta time: " +
                       std::to_string(deltaTime) + " seconds.");
        }

        if (sceneManager.sceneCount() == 0) {
            FORGE_WARN("No active scenes remaining. Exiting main loop.");
            break;
        }
    }
    FORGE_INFO("Exiting main loop.");
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

    FORGE_INFO("Engine shutdown complete.");
    Log::shutdown();
}
}  // namespace Forge
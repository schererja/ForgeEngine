#pragma once

#include <string>

#include "AssetManager.h"
#include "AudioSystem.h"
#include "Camera.h"
#include "Components.h"
#include "EntityManager.h"
#include "Input.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Window.h"
namespace Forge {

// Runtime configuration for creating the main game window.
struct EngineConfig {
    std::string windowTitle = "Forge Engine";
    int windowWidth = 1280;
    int windowHeight = 720;
};

// High-level engine facade that owns windowing and rendering systems.
class Engine {
   public:
    Engine() = default;
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    // Creates core systems and prepares the engine to run.
    bool initialize(const EngineConfig& config = EngineConfig());

    // Runs the main loop until the window closes.
    void run();

    // Releases all owned systems.
    void shutdown();

    // Accessors for core systems. These are non-owning raw pointers for
    // simplicity.
    EntityManager& getEntityManager() { return entityManager; }
    AssetManager& getAssetManager() { return assetManager; }
    SceneManager& getSceneManager() { return sceneManager; }
    Renderer& getRenderer() { return *renderer; }
    Input& getInput() { return *input; }
    Camera& getCamera() { return camera; }
    AudioSystem& getAudio() { return audioSystem; }
    int getWidth() { return width; }
    int getHeight() { return height; }

   private:
    Window* window = nullptr;
    Renderer* renderer = nullptr;
    Input* input = nullptr;
    EntityManager entityManager;
    AssetManager assetManager;
    SceneManager sceneManager;
    AudioSystem audioSystem;
    Camera camera{1280, 720};
    int width = 1280;
    int height = 720;
};
}  // namespace Forge
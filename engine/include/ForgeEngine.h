#pragma once

#include <string>

#include "Camera.h"
#include "EntityManager.h"
#include "Input.h"
#include "Renderer.h"
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

    // Accessors for core systems. These are non-owning raw pointers for simplicity.
    EntityManager& getEntityManager() { return entityManager; }

   private:
    Window* window = nullptr;
    Renderer* renderer = nullptr;
    Input* input = nullptr;
    EntityManager entityManager;
};
}  // namespace Forge
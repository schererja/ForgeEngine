#pragma once

#include <string>

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
    // Creates core systems and prepares the engine to run.
    bool initialize(const EngineConfig& config = EngineConfig());

    // Runs the main loop until the window closes.
    void run();

    // Releases all owned systems.
    void shutdown();

   private:
    Window* window = nullptr;
    Renderer* renderer = nullptr;
    Input* input = nullptr;
};
}  // namespace Forge
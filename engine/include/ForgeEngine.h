#pragma once

#include "Window.h"
#include "Renderer.h"
#include <string>

namespace Forge {

    struct EngineConfig {
        std::string windowTitle = "Forge Engine";
        int windowWidth = 1280;
        int windowHeight = 720;
    };
    class Engine {
        public:
            bool initialize(const EngineConfig& config = EngineConfig());
            void run();
            void shutdown();
        private:
            Window* window = nullptr;
            Renderer* renderer = nullptr;
    };
}
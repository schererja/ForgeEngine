#include <ForgeEngine.h>

#include "OverworldScene.h"
int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    Forge::Engine engine;
    Forge::EngineConfig config;
    config.windowTitle = "Ember";
    config.windowWidth = 800;
    config.windowHeight = 600;

    if (!engine.initialize(config)) {
        return 1;
    }
    // push initial scene
    engine.getSceneManager().push(std::make_unique<OverworldScene>(), engine);
    engine.run();
    engine.shutdown();
    return 0;
}
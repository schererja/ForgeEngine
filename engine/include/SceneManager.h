#pragma once

#include <memory>
#include <vector>

#include "Scene.h"

namespace Forge {
class Engine;
class SceneManager {
   public:
    // Push a new scene ontop of the stack
    void push(std::unique_ptr<Scene> scene, Engine& engine);

    // Pop the current scene off the stack
    void pop(Engine& engine);

    // Replace the top scene with a new one
    void replace(std::unique_ptr<Scene> scene, Engine& engine);

    // Update the active scene(s). Scenes are updated from top to bottom until a
    // scene returns false from updatesBelow(), allowing for pause menus or
    // overlays.
    void update(Engine& engine, float deltaTime);

    // Render the active scene(s). Scenes are rendered from bottom to top until
    // a scene returns false from rendersBelow(), allowing for pause menus or
    // overlays.
    void render(Engine& engine);

    // Is there any scene currently active?
    bool hasActiveScene() const { return scenes.empty(); }

    // How many scenes do we have on the stack?
    size_t sceneCount() const { return scenes.size(); }

   private:
    std::vector<std::unique_ptr<Scene>> scenes;
};
}  // namespace Forge

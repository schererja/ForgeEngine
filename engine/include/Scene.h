#pragma once

namespace Forge {
class Engine;
class Scene {
   public:
    virtual ~Scene() = default;
    virtual void onEnter(Engine& engine) = 0;
    // Called when the scene is being switched out, allowing for cleanup or state saving.
    virtual void onExit(Engine& engine) = 0;
    // update game logic, called every frame with delta time in seconds
    virtual void update(Engine& engine, float deltaTime) = 0;
    // render the scene, called every frame after update
    virtual void render(Engine& engine) = 0;
    // Should the scene below this one still render?
    // Useful for pause menus or overlays.
    virtual bool rendersBelow() const { return false; }
    // Should the scene below this one still update?
    virtual bool updatesBelow() const { return false; }
};
}  // namespace Forge
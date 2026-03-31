#include "SceneManager.h"

#include <iostream>

#include "Log.h"
namespace Forge {
void SceneManager::push(std::unique_ptr<Scene> scene, Engine& engine) {
    scene->onEnter(engine);
    scenes.push_back(std::move(scene));
    FORGE_INFO("Pushed new scene. Total scenes: " +
               std::to_string(scenes.size()));
}

void SceneManager::pop(Engine& engine) {
    if (scenes.empty()) {
        FORGE_ERROR("Cannot pop scene: no active scenes.");

        return;
    }
    scenes.back()->onExit(engine);
    scenes.pop_back();
    FORGE_INFO("Popped scene. Total scenes: " + std::to_string(scenes.size()));

    // Resume the scene below if there is one
    if (!scenes.empty()) {
        FORGE_INFO("Resuming scene below. Total scenes: " +
                   std::to_string(scenes.size()));
    }
}

void SceneManager::replace(std::unique_ptr<Scene> scene, Engine& engine) {
    if (!scenes.empty()) {
        scenes.back()->onExit(engine);
        scenes.pop_back();
    }
    scene->onEnter(engine);
    scenes.push_back(std::move(scene));
    FORGE_INFO("Replaced top scene. Total scenes: " +
               std::to_string(scenes.size()));
}
void SceneManager::update(Engine& engine, float deltaTime) {
    if (scenes.empty()) {
        return;
    }

    // Find how far down the stack we should update based on updatesBelow()
    int updateIndex = static_cast<int>(scenes.size()) - 1;
    while (updateIndex > 0 && scenes[updateIndex]->updatesBelow()) {
        updateIndex--;
    }

    // Update from the bottom of the stack up to the determined index
    for (size_t i = updateIndex; i < scenes.size(); i++) {
        scenes[i]->update(engine, deltaTime);
    }
}

void SceneManager::render(Engine& engine) {
    if (scenes.empty()) {
        return;
    }

    // Find how far down the stack we need to render
    int startIndex = static_cast<int>(scenes.size()) - 1;
    while (startIndex > 0 && scenes[startIndex]->rendersBelow()) {
        startIndex--;
    }
    // Render from the determined index up to the top of the stack
    for (size_t i = startIndex; i < scenes.size(); i++) {
        scenes[i]->render(engine);
    }
}
}  // namespace Forge
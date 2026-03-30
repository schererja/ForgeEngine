#include "SceneManager.h"

#include <iostream>

namespace Forge {
void SceneManager::push(std::unique_ptr<Scene> scene, Engine& engine) {
    scene->onEnter(engine);
    scenes.push_back(std::move(scene));
    std::cout << "[SceneManager] Pushed new scene. Total scenes: "
              << scenes.size() << std::endl;
}

void SceneManager::pop(Engine& engine) {
    if (scenes.empty()) {
        std::cerr << "[SceneManager] Cannot pop scene: no active scenes."
                  << std::endl;
        return;
    }
    scenes.back()->onExit(engine);
    scenes.pop_back();
    std::cout << "[SceneManager] Popped scene. Total scenes: " << scenes.size()
              << std::endl;
    // Resume the scene below if there is one
    if (!scenes.empty()) {
        std::cout << "[SceneManager] Resuming scene below. Total scenes: "
                  << scenes.size() << std::endl;
    }
}

void SceneManager::replace(std::unique_ptr<Scene> scene, Engine& engine) {
    if (!scenes.empty()) {
        scenes.back()->onExit(engine);
        scenes.pop_back();
    }
    scene->onEnter(engine);
    scenes.push_back(std::move(scene));
    std::cout << "[SceneManager] Replaced top scene. Total scenes: "
              << scenes.size() << std::endl;
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
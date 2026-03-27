#include "AssetManager.h"

#include <iostream>

namespace Forge {

Texture* AssetManager::getTexture(const std::string& filePath) {  // Check the cache first
    auto it = textures.find(filePath);
    if (it != textures.end()) {
        return it->second.get();
    }

    // Load the texture and cache it
    std::cout << "[FORGE] Loading texture: " << filePath << std::endl;
    auto texture = std::make_unique<Texture>(filePath);
    if (!texture->isValid()) {
        std::cerr << "[FORGE] Failed to load texture: " << filePath << std::endl;
        return nullptr;
    }
    Texture* texturePtr = texture.get();
    textures[filePath] = std::move(texture);
    return texturePtr;
};

void AssetManager::unloadTexture(const std::string& filePath) {
    auto it = textures.find(filePath);
    if (it != textures.end()) {
        std::cout << "[FORGE] Unloading texture: " << filePath << std::endl;
        textures.erase(it);
    }
}

void AssetManager::unloadAll() {
    std::cout << "[FORGE] Unloading all assets" << std::endl;
    textures.clear();
}
}  // namespace Forge
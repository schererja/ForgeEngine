#include "AssetManager.h"

#include "Log.h"

namespace Forge {

Texture* AssetManager::getTexture(
    const std::string& filePath) {  // Check the cache first
    auto it = textures.find(filePath);
    if (it != textures.end()) {
        return it->second.get();
    }

    // Load the texture and cache it
    FORGE_INFO("Loading texture: {}", filePath);

    auto texture = std::make_unique<Texture>(filePath);
    if (!texture->isValid()) {
        FORGE_ERROR("Failed to load texture: {}", filePath);
        return nullptr;
    }
    Texture* texturePtr = texture.get();
    textures[filePath] = std::move(texture);
    return texturePtr;
};

void AssetManager::unloadTexture(const std::string& filePath) {
    auto it = textures.find(filePath);
    if (it != textures.end()) {
        FORGE_WARN("Unloading texture: {}", filePath);

        textures.erase(it);
    }
}

void AssetManager::unloadAll() {
    FORGE_INFO("Unloading all assets");
    textures.clear();
}
}  // namespace Forge
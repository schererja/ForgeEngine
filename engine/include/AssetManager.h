#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Texture.h"

namespace Forge {
class AssetManager {
   public:
    AssetManager() = default;
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    // Get a texture by file path. Loads it if not already loaded.
    Texture* getTexture(const std::string& filePath);

    // Unload a texture from memory. This will remove it from the cache and free GPU resources.
    void unloadTexture(const std::string& filePath);

    // Unload all assets from memory. This is useful for cleanup when shutting down the engine.
    void unloadAll();

    // How many textures are currently loaded (for debugging/analytics).
    size_t getTextureCount() const { return textures.size(); }

   private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};
}  // namespace Forge
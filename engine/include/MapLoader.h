#pragma once

#include <sol/sol.hpp>
#include <string>
#include <unordered_set>

#include "Components.h"
#include "EntityManager.h"
#include "Log.h"
#include "Tilemap.h"

namespace Forge {
struct MapData {
    Tilemap tilemap;
    std::string name;
    int version = 0;

    // Spawn points parsed from map file
    struct EntitySpawn {
        std::string type;
        float x = 0, y = 0;
        float width = 32.0f, height = 32.0f;
        std::string name;
        std::string script;
    };

    struct Trigger {
        int x = 0, y = 0;
        int width = 1, height = 1;
        std::string script;
    };

    std::vector<EntitySpawn> spawns;
    std::vector<Trigger> triggers;
    bool valid = false;
};

class MapLoader {
   public:
    // Load a map from a lua file
    // Returns with valid=false if loading fails
    static MapData loadFromFile(
        const std::string& path,
        const std::string& assetBasePath = "../game/assets/");

   private:
    static bool validateMapTable(const sol::table& mapTable,
                                 const std::string& path);
};
}  // namespace Forge
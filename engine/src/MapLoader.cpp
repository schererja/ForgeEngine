#include "MapLoader.h"

#include <unordered_set>

namespace Forge {
MapData MapLoader::loadFromFile(const std::string& path,
                                const std::string& assetBasePath) {
    MapData result;
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    auto loadResult = lua.safe_script_file(path, sol::script_pass_on_error);
    if (!loadResult.valid()) {
        sol::error err = loadResult;
        FORGE_ERROR("Failed to load map file '{}': {}", path, err.what());
        return result;
    }
    sol::table map = lua["map"];
    if (!map.valid()) {
        FORGE_ERROR("Map file '{}' does not contain a 'map' table", path);
        return result;
    }
    // Validate the required fields
    if (!validateMapTable(map, path)) {
        return result;
    }

    // Read the metadata
    result.name = map["name"].get_or(std::string("Unnamed Map"));
    result.version = map["version"].get_or(0);
    // Load the tilemap
    int mapWidth = map["width"];
    int mapHeight = map["height"];
    int tileWidth = map["tileWidth"];
    int tileHeight = map["tileHeight"];

    // Build tileset
    sol::table tilesetTable = map["tileset"];
    Tileset tileset;
    tileset.texturePath =
        assetBasePath + tilesetTable["path"].get<std::string>();
    tileset.tileWidth = tileWidth;
    tileset.tileHeight = tileHeight;
    tileset.columns = tilesetTable["columns"];
    tileset.rows = tilesetTable["rows"];

    // Build solid tile set
    std::unordered_set<int> solidTileIDs;
    sol::table solidTable = map["solidTiles"];
    for (auto& [key, val] : solidTable) {
        solidTileIDs.insert(val.as<int>());
    }

    // Build tilemap
    result.tilemap = Tilemap(mapWidth, mapHeight, tileWidth, tileHeight);
    result.tilemap.setTileset(tileset);

    sol::table tiles = map["tiles"];
    int expectedCount = mapWidth * mapHeight;
    int actualCount = 0;

    for (auto& [key, val] : tiles) {
        actualCount++;
    }
    if (actualCount != expectedCount) {
        FORGE_ERROR("Map file '{}' has {} tiles, but expected {}", path,
                    actualCount, expectedCount);
        return result;
    }

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int index =
                y * mapWidth + x +
                1;  // Lua tables are 1-indexed, so we adjust by subtracting 1
            int tileID = tiles[index];
            bool solid = solidTileIDs.count(tileID) > 0;
            result.tilemap.setTile(x, y, tileID, solid);
        }
    }

    // Parse entity spawns
    sol::optional<sol::table> entitiesOpt = map["entities"];
    if (entitiesOpt) {
        for (auto& [key, val] : *entitiesOpt) {
            sol::table entityTable = val;
            MapData::EntitySpawn spawn;
            spawn.type = entityTable["type"].get_or(std::string("Unknown"));
            spawn.x = entityTable["x"].get_or(0.0f);
            spawn.y = entityTable["y"].get_or(0.0f);
            spawn.width = entityTable["width"].get_or(32.0f);
            spawn.height = entityTable["height"].get_or(32.0f);
            spawn.name = entityTable["name"].get_or(std::string(""));
            spawn.script = entityTable["script"].get_or(std::string(""));
            result.spawns.push_back(spawn);
        }
    }

    // Parse triggers
    sol::optional<sol::table> triggersOpt = map["triggers"];
    if (triggersOpt) {
        for (auto& [key, val] : *triggersOpt) {
            sol::table t = val;
            MapData::Trigger trigger;
            trigger.x = t["x"].get_or(0);
            trigger.y = t["y"].get_or(0);
            trigger.width = t["width"].get_or(1);
            trigger.height = t["height"].get_or(1);
            trigger.script = t["script"].get_or(std::string(""));
            result.triggers.push_back(trigger);
        }
    }

    result.valid = true;
    FORGE_INFO("Loaded map: {} ({}x{}, {} entities, {} triggers)", result.name,
               mapWidth, mapHeight, result.spawns.size(),
               result.triggers.size());

    return result;
};

bool MapLoader::validateMapTable(const sol::table& map,
                                 const std::string& path) {
    // Check all required fields exist
    bool valid = true;

    // Helper lambda to check if a field exists and is valid
    auto check = [&](const std::string& field) {
        if (!map[field].valid()) {
            FORGE_ERROR("Map {} missing required field: '{}'", path, field);
            valid = false;
        }
    };

    check("width");
    check("height");
    check("tileWidth");
    check("tileHeight");
    check("tileset");
    check("tiles");
    check("solidTiles");

    if (!valid) return false;

    // Validate tileset has required fields
    sol::table tileset = map["tileset"];
    if (!tileset["path"].valid()) {
        FORGE_ERROR("Map {} tileset missing 'path'", path);
        return false;
    }
    if (!tileset["columns"].valid() || !tileset["rows"].valid()) {
        FORGE_ERROR("Map {} tileset missing 'columns' or 'rows'", path);
        return false;
    }

    return true;
};
}  // namespace Forge
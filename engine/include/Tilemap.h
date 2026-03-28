#pragma once

#include <string>
#include <vector>

namespace Forge {
struct Tile {
    int tileID = 0;
    bool solid = false;
    bool visible = true;
};

struct Tileset {
    std::string texturePath;
    int tileWidth = 32;
    int tileHeight = 32;
    int columns = 0;
    int rows = 0;
};
class Tilemap {
   public:
    Tilemap() = default;
    Tilemap(int mapWidth, int mapHeight, int tileWidth, int tileHeight);

    // Tile access
    Tile& getTile(int x, int y);
    const Tile& getTile(int x, int y) const;
    void setTile(int x, int y, int tileID, bool solid = false);

    // Fill map with one tile
    void fill(int tileID, bool solid = false);

    // Bounds checking
    bool isInBounds(int x, int y) const;

    // Getters
    int getMapWidth() const { return mapWidth; }
    int getMapHeight() const { return mapHeight; }
    int getTileWidth() const { return tileWidth; }
    int getTileHeight() const { return tileHeight; }

    // World size in pixels
    float getWorldWidth() const { return mapWidth * tileWidth; }
    float getWorldHeight() const { return mapHeight * tileHeight; }

    // Tileset
    void setTileset(const Tileset& ts) { tileset = ts; }
    const Tileset& getTileset() const { return tileset; }

   private:
    int mapWidth = 0;
    int mapHeight = 0;
    int tileWidth = 32;
    int tileHeight = 32;

    std::vector<Tile> tiles;
    Tileset tileset;
};
}  // namespace Forge
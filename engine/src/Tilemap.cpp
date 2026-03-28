#include "Tilemap.h"

#include <iostream>
#include <stdexcept>

namespace Forge {

Tilemap::Tilemap(int mapWidth, int mapHeight, int tileWidth, int tileHeight)
    : mapWidth(mapWidth), mapHeight(mapHeight), tileWidth(tileWidth), tileHeight(tileHeight) {
    tiles.resize(mapWidth * mapHeight);
}

Tile& Tilemap::getTile(int x, int y) {
    if (!isInBounds(x, y)) {
        throw std::out_of_range("Tile coordinates out of bounds");
    }
    return tiles[y * mapWidth + x];
}

const Tile& Tilemap::getTile(int x, int y) const {
    if (!isInBounds(x, y)) {
        throw std::out_of_range("Tile coordinates out of bounds");
    }
    return tiles[y * mapWidth + x];
}

void Tilemap::setTile(int x, int y, int tileID, bool solid) {
    if (!isInBounds(x, y)) {
        throw std::out_of_range("Tile coordinates out of bounds");
    }
    tiles[y * mapWidth + x].tileID = tileID;
    tiles[y * mapWidth + x].solid = solid;
}

void Tilemap::fill(int tileID, bool solid) {
    for (auto& tile : tiles) {
        tile.tileID = tileID;
        tile.solid = solid;
    }
}

bool Tilemap::isInBounds(int x, int y) const {
    return x >= 0 && x < mapWidth && y >= 0 && y < mapHeight;
}
}  // namespace Forge
#include "EntityManager.h"

namespace Forge {

EntityID EntityManager::createEntity() {
    EntityID id;
    if (!recycledIDs.empty()) {
        id = recycledIDs.back();
        recycledIDs.pop();
    } else {
        id = nextEntityID++;
    }
    aliveEntities[id] = true;
    return id;
}

void EntityManager::destroyEntity(EntityID id) {
    if (!isEntityAlive(id)) return;

    // Remove all components associated with this entity
    for (auto& [type, componentStore] : componentStores) {
        componentStore->removeComponent(id);
    }

    aliveEntities.erase(id);
    recycledIDs.push(id);  // Recycle the ID for future use
}

bool EntityManager::isEntityAlive(EntityID id) const { return aliveEntities.count(id) > 0; }
}  // namespace Forge
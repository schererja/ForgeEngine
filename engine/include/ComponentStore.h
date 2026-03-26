#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>

namespace Forge {

using EntityID = uint32_t;
const EntityID NULL_ENTITY = 0;
// A simple component store that allows adding, removing, and retrieving components of any type
// associated with an entity ID. Uses type erasure to store components of different types
// in a single map.
class IComponentStore {
   public:
    virtual ~IComponentStore() = default;
    virtual void removeComponent(EntityID id) = 0;
};

template <typename T>
class ComponentStore : public IComponentStore {
   public:
    // Adds or replaces a component for the given entity ID.
    void addComponent(EntityID entityID, T component) {
        components[entityID] = std::move(component);
    }

    // Removes the component associated with the given entity ID.
    void removeComponent(EntityID entityID) { components.erase(entityID); }

    // Get a pointer to the component associated with the given entity ID, or nullptr if it doesn't
    // exist.
    T* getComponent(EntityID entityID) {
        auto it = components.find(entityID);
        if (it != components.end()) {
            return &it->second;
        }
        return nullptr;
    }
    // Iterate over all components and their associated entity IDs, applying the given function.
    void forEach(std::function<void(EntityID, T&)> func) {
        for (auto& pair : components) {
            func(pair.first, pair.second);
        }
    }

    // Direct access to the underlying map for advanced use cases (e.g. serialization).
    std::unordered_map<EntityID, T>& getAllComponents() { return components; }

   private:
    std::unordered_map<EntityID, T> components;
};
}  // namespace Forge
#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <typeindex>
#include <unordered_map>

#include "ComponentStore.h"

namespace Forge {

class EntityManager {
   public:
    EntityManager() = default;

    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;

    // Movable but not copyable
    EntityManager(EntityManager&&) = default;
    EntityManager& operator=(EntityManager&&) = default;

    // Lifecycle of the entities
    EntityID createEntity();
    void destroyEntity(EntityID id);
    bool isEntityAlive(EntityID id) const;

    // Component management
    template <typename T>
    void addComponent(EntityID id, T component) {
        getStore<T>()->addComponent(id, std::move(component));
    }
    template <typename T>
    T* getComponent(EntityID id) {
        return getStore<T>()->getComponent(id);
    }
    template <typename T>
    void removeComponent(EntityID id) {
        getStore<T>()->removeComponent(id);
    }

    // Iterate over all entities with a component of type T and apply the given function.
    template <typename T>
    void forEach(std::function<void(EntityID, T&)> func) {
        getStore<T>()->forEach(func);
    }

    // Iterate all entities that have T1, T2 apply the given function.
    template <typename T1, typename T2>
    void forEach(std::function<void(EntityID, T1&, T2&)> func) {
        auto store1 = getStore<T1>();
        auto store2 = getStore<T2>();

        // For simplicity, we iterate over the smaller store to minimize lookups.
        if (store1->getAllComponents().size() < store2->getAllComponents().size()) {
            for (auto& pair : store1->getAllComponents()) {
                EntityID id = pair.first;
                T1& comp1 = pair.second;
                T2* comp2 = store2->getComponent(id);
                if (comp2) {
                    func(id, comp1, *comp2);
                }
            }
        } else {
            for (auto& pair : store2->getAllComponents()) {
                EntityID id = pair.first;
                T2& comp2 = pair.second;
                T1* comp1 = store1->getComponent(id);
                if (comp1) {
                    func(id, *comp1, comp2);
                }
            }
        }
    }

    // Iterate all entities that have T1, T2, T3 apply the given function.
    template <typename T1, typename T2, typename T3>
    void forEach(std::function<void(EntityID, T1&, T2&, T3&)> func) {
        auto store1 = getStore<T1>();
        auto store2 = getStore<T2>();
        auto store3 = getStore<T3>();

        // Iterate over the smallest store to minimize lookups.
        if (store1->getAllComponents().size() < store2->getAllComponents().size() &&
            store1->getAllComponents().size() < store3->getAllComponents().size()) {
            for (auto& pair : store1->getAllComponents()) {
                EntityID id = pair.first;
                T1& comp1 = pair.second;
                T2* comp2 = store2->getComponent(id);
                T3* comp3 = store3->getComponent(id);
                if (comp2 && comp3) {
                    func(id, comp1, *comp2, *comp3);
                }
            }
        } else if (store2->getAllComponents().size() < store3->getAllComponents().size()) {
            for (auto& pair : store2->getAllComponents()) {
                EntityID id = pair.first;
                T2& comp2 = pair.second;
                T1* comp1 = store1->getComponent(id);
                T3* comp3 = store3->getComponent(id);
                if (comp1 && comp3) {
                    func(id, *comp1, comp2, *comp3);
                }
            }
        } else {
            for (auto& pair : store3->getAllComponents()) {
                EntityID id = pair.first;
                T3& comp3 = pair.second;
                T1* comp1 = store1->getComponent(id);
                T2* comp2 = store2->getComponent(id);
                if (comp1 && comp2) {
                    func(id, *comp1, *comp2, comp3);
                }
            }
        }
    }

    // Get total number of alive entities (for debugging/analytics).
    size_t getEntityCount() const { return aliveEntities.size(); };

   private:
    EntityID nextEntityID = 1;         // Start from 1 to avoid using NULL_ENTITY as 0
    std::queue<EntityID> recycledIDs;  // IDs that have been destroyed and can be reused
    std::unordered_map<EntityID, bool> aliveEntities;  // Track which entities are currently alive

    // Type-erased map of component stores, keyed by type_index of the component type.
    std::unordered_map<std::type_index, std::unique_ptr<IComponentStore>> componentStores;

    template <typename T>
    ComponentStore<T>* getStore() {
        auto key = std::type_index(typeid(T));
        auto it = componentStores.find(key);
        if (it == componentStores.end()) {
            // Create a new store for this component type if it doesn't exist.
            componentStores[key] = std::make_unique<ComponentStore<T>>();
        }
        return static_cast<ComponentStore<T>*>(componentStores[key].get());
    }
};
}  // namespace Forge
#pragma once

#include "Entity.h"
#include <unordered_set>

class EntityManager {
public:
    EntityID CreateEntity() {
        EntityID id = NextID++;
        alive.insert(id);
        return id;
    }

    void DestroyEntity(EntityID id) {
        alive.erase(id);
    }

    bool IsAlive(EntityID id) {
        return alive.find(id) != alive.end();
    }

    std::unordered_set<EntityID> GetAllEntities() {
        return alive;
    }

private:
    EntityID NextID = 0;
    std::unordered_set<EntityID> alive;
};
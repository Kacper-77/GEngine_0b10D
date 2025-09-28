#pragma once

#include "Entity.h"
#include <string>
#include <unordered_set>
#include <unordered_map>

class EntityManager {
public:
    EntityID CreateEntity() {
        EntityID id = NextID++;
        alive.insert(id);
        return id;
    }

    void DestroyEntity(EntityID id) {
        alive.erase(id);
        RemoveTag(id);
    }

    bool IsAlive(EntityID id) {
        return alive.find(id) != alive.end();
    }

    std::unordered_set<EntityID> GetAllEntities() {
        return alive;
    }

    void AddTag(EntityID id, const std::string& tag) {
        tags[id] = tag;
        groups[tag].insert(id);
    }

    void RemoveTag(EntityID id) {
        auto it = tags.find(id);
        if (it != tags.end()) {
            groups[it->second].erase(id);
            tags.erase(it);
        }
    }

    std::string GetTag(EntityID id) const {
        auto it = tags.find(id);
        return it != tags.end() ? it->second : "";
    }

    std::unordered_set<EntityID> GetGroup(const std::string& tag) const {
        auto it = groups.find(tag);
        return it != groups.end() ? it->second : std::unordered_set<EntityID>{};
    }

private:
    EntityID NextID = 0;
    std::unordered_set<EntityID> alive;
    std::unordered_map<EntityID, std::string> tags;
    std::unordered_map<std::string, std::unordered_set<EntityID>> groups;
};
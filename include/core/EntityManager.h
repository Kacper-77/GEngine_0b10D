#pragma once

#include "utils/EntityTypes.h"
#include "ComponentStorage.h"
#include <string>
#include <unordered_set>
#include <unordered_map>

using EntityInfo = std::unordered_map<std::string, std::string>;

class EntityManager {
public:
    // Add to list of "alive"  entities
    void InsertToEntityList(EntityID id) {
        alive.insert(id);
    }

    // Register ComponentStorage
    void RegisterComponentStorage(IComponentStorage *storage) { 
        componentStorage.push_back(storage); 
    }

    // Destroy and remove entity from all fields
    void DestroyEntityFromList(EntityID id) {
        alive.erase(id);
        RemoveTag(id);
        for (auto* storage : componentStorage) {
            storage->Remove(id);
        }
        RemoveInfo(id);
    }

    // Check entity
    bool IsAlive(EntityID id) {
        return alive.find(id) != alive.end();
    }

    // Getting all entities
    std::unordered_set<EntityID> GetAllEntities() {
        return alive;
    }

    // Add, remove and get entity tag 
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

    // Get group with same tags (example - "Enemy" etc.)
    std::unordered_set<EntityID> GetGroup(const std::string& tag) const {
        auto it = groups.find(tag);
        return it != groups.end() ? it->second : std::unordered_set<EntityID>{};
    }

    // Add, get and remove info about entity
    void AddInfo(EntityID id, const std::string& key, const std::string& value) {
        m_entityInfo[id][key] = value;
    }

    std::string GetInfo(EntityID id, const std::string& key) const {
        auto it = m_entityInfo.find(id);
        if (it != m_entityInfo.end()) {
            auto fit = it->second.find(key);
            return fit != it->second.end() ? fit->second : "";
        }
        return "";
    }

    void RemoveInfo(EntityID id) {
        m_entityInfo.erase(id);
    }

private:
    std::unordered_set<EntityID> alive;
    std::unordered_map<EntityID, std::string> tags;
    std::unordered_map<std::string, std::unordered_set<EntityID>> groups;
    std::vector<IComponentStorage*> componentStorage;
    std::unordered_map<EntityID, EntityInfo> m_entityInfo;
};
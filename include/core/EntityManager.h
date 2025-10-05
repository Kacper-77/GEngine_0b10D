#pragma once

#include "utils/EntityTypes.h"
#include "ComponentStorage.h"
#include <string>
#include <unordered_set>
#include <unordered_map>

using EntityInfo = std::unordered_map<std::string, std::string>;

class EntityManager {
public:
    EntityID CreateEntity() {
        EntityID id = NextID_++;
        alive.insert(id);
        return id;
    }

    void RegisterComponentStorage(IComponentStorage *storage) { 
        componentStorage.push_back(storage); 
    }

    void DestroyEntity(EntityID id) {
        alive.erase(id);
        RemoveTag(id);
        for (auto* storage : componentStorage) {
            storage->Remove(id);
        }
        RemoveInfo(id);
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
    EntityID NextID_ = 1;
    std::unordered_set<EntityID> alive;
    std::unordered_map<EntityID, std::string> tags;
    std::unordered_map<std::string, std::unordered_set<EntityID>> groups;
    std::vector<IComponentStorage*> componentStorage;
    std::unordered_map<EntityID, EntityInfo> m_entityInfo;
};
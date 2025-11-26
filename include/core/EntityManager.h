#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>


#include "utils/Vector.h"
#include "utils/EntityTypes.h"
#include "systems/ItemsDropsSystem.h"
#include "ComponentStorage.h"

using EntityInfo = std::unordered_map<std::string, std::string>;

struct RespawnInfo {
    VectorFloat position;
    float respawnDelay;
};

class EntityManager {
public:
    // Add to list of "alive" entities
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
    bool IsAlive(EntityID id) const {
        return alive.find(id) != alive.end();
    }

    // Getting all entities
    const std::unordered_set<EntityID>& GetAllEntities() {
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
    const std::unordered_set<EntityID>& GetGroup(const std::string& tag) const {
        static const std::unordered_set<EntityID> empty;
        auto it = groups.find(tag);
        return it != groups.end() ? it->second : empty;
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

    // Respawn
    void SetRespawnPoint(EntityID id,const RespawnInfo& respawn) {
        m_respawnPoint[id] = respawn;
    }

    RespawnInfo GetRespawnPoint(EntityID id) {
        auto it = m_respawnPoint.find(id);
        return it != m_respawnPoint.end() ? it->second : RespawnInfo{};
    }

    // Drop
    void AddDrop(EntityID id, const DropInfo& drop) {
        m_entityDrops[id].push_back(drop);
    }

    const std::vector<DropInfo>& GetDrops(EntityID id) const {
        static const std::vector<DropInfo> empty;
        auto it = m_entityDrops.find(id);
        return it != m_entityDrops.end() ? it->second : empty;
    }

    void ClearDrops(EntityID id) {
        m_entityDrops.erase(id);
    }

private:
    std::unordered_set<EntityID> alive;
    std::unordered_map<EntityID, std::string> tags;
    std::unordered_map<std::string, std::unordered_set<EntityID>> groups;
    std::vector<IComponentStorage*> componentStorage;
    std::unordered_map<EntityID, EntityInfo> m_entityInfo;
    
    std::unordered_map<EntityID, RespawnInfo> m_respawnPoint;
    std::unordered_map<EntityID, std::vector<DropInfo>> m_entityDrops;
};
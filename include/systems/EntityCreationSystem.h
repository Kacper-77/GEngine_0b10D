#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>

#include "core/ISystem.h"
#include "core/EntityManager.h"
#include "utils/EntityTypes.h"

class EntityCreationSystem : public ISystem {
public:
    EntityCreationSystem(EntityManager* manager)
        : m_manager{manager} {}

    // Create entities with registred components
    template<typename... Args>
    EntityID CreateEntityWith(Args&&... args) {
        EntityID id = GetIdentificator();
        m_manager->InsertToEntityList(id);
        (ProcessArgument(id, std::forward<Args>(args)), ...);
        return id;
    }

    // No arguments entity
    EntityID CreateEntity() {
        EntityID id = GetIdentificator();
        m_manager->InsertToEntityList(id);
        return id;
    }

    // Component registration
    template<typename T>
    void RegisterStorage(ComponentStorage<T>* storage) {
        m_storages[typeid(T)] = storage;
    }

private:
    EntityID GetIdentificator() {
        return NextID_++;
    }

    // Processing arguments depending by type
    template<typename T>
    void ProcessArgument(EntityID id, T&& arg) {
        if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
            m_manager->AddTag(id, std::forward<T>(arg));
        }
        else if constexpr (std::is_same_v<std::decay_t<T>, EntityInfo>) {
            for (const auto& [key, value] : arg) {
                m_manager->AddInfo(id, key, value);
            }
        }
        else {
            auto* storage = GetStorage<std::decay_t<T>>();
            if (storage) {
                storage->Add(id, std::forward<T>(arg));
            }
        }
    }

    // Extract pointer to registered storage
    template<typename T>
    ComponentStorage<T>* GetStorage() {
        auto it = m_storages.find(typeid(T));
        return it != m_storages.end() ? static_cast<ComponentStorage<T>*>(it->second) : nullptr;
    }

    EntityID NextID_ = 1;
    EntityManager* m_manager;
    std::unordered_map<std::type_index, IComponentStorage*> m_storages;
    void Update(float deltaTime) override {};  // Only for World class
};

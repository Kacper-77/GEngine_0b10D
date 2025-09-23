#pragma once
#include <unordered_map>
#include "Entity.h"

template<typename T>
class ComponentStorage {
public:
    void Add(EntityID id, const T& component) {
        m_components[id] = component;
    }

    T* Get(EntityID id) {
        auto it = m_components.find(id);
        return it != m_components.end() ? &it->second : nullptr;
    }

    void Remove(EntityID id) {
        auto it = m_components.find(id);
        if (it != m_components.end()) {
            m_components.erase(it);
        }
    }

    std::unordered_map<EntityID, T>& GetAll() {
        return m_components;
    }

private:
    std::unordered_map<EntityID, T> m_components;
};

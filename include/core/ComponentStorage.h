#pragma once

#include "IComponentStorage.h"
#include <unordered_map>
#include <vector>

template<typename T>
class ComponentStorage : public IComponentStorage {
public:
    void Add(EntityID id, const T& component) {
        m_components[id] = component;
    }

    T* Get(EntityID id) {
        auto it = m_components.find(id);
        return it != m_components.end() ? &it->second : nullptr;
    }

    bool Has(EntityID id) const {
        return m_components.find(id) != m_components.end();
    }

    void Remove(EntityID id) override {
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

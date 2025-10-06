#pragma once

#include <vector>
#include <memory>
#include "core/ISystem.h"

class SystemManager {
public:
    template<typename T, typename... Args>
    void RegisterSystem(Args&&... args) {
        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        m_systems.push_back(std::move(system));
    }

    void UpdateAll(float deltaTime) {
        for (auto& system : m_systems) {
            system->Update(deltaTime);
        }
    }

    template<typename T>
    T* GetSystem() {
        for (auto& system : m_systems) {
            if (auto ptr = dynamic_cast<T*>(system.get())) {
                return ptr;
            }
        }
        return nullptr;
    }

private:
    std::vector<std::unique_ptr<ISystem>> m_systems;
};

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

    void UpdateAll(float deltaTime);

private:
    std::vector<std::unique_ptr<ISystem>> m_systems;
};

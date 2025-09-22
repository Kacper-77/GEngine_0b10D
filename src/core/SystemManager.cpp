#include "core/SystemManager.h"

void SystemManager::UpdateAll(float deltaTime) {
    for (auto& system : m_systems) {
        system->Update(deltaTime);
    }
}

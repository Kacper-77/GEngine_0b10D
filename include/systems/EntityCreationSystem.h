#pragma once

#include <vector>
#include <memory>
#include "core/EntityManager.h"
#include "utils/EntityTypes.h"

class EntityCreationSystem {
public:
    EntityCreationSystem(EntityManager* manager)
        : m_manager{manager} {}

    template<typename... Args>
     EntityID CreateEntityWith(Args&&... args) {
        EntityID id = m_manager->CreateEntity();
    }

private:
    EntityManager* m_manager;
};
#pragma once

#include "core/ISystem.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include "components/ColliderComponent.h"
#include "event/core/EventManager.h"
#include "event/core/EventFactory.h"
#include "../utils/EventResolver.h"

#include <functional>

class CollisionSystem : public ISystem {
public:
    CollisionSystem(EntityManager& entityManager,
                    ComponentStorage<ColliderComponent>& colliders,
                    EventManager& eventManager,
                    EventFactory& eventFactory,
                    EventResolver& eventResolver);
    
    void Update(float deltaTime) override;

private:
    bool IsColliding(const ColliderComponent& a, const ColliderComponent& b);
    void HandleCollision(EntityID a, EntityID b, const std::string& eventType);

    EntityManager& m_entityManager;
    ComponentStorage<ColliderComponent>& m_colliders;
    EventManager& m_eventManager;
    EventFactory& m_eventFactory;
    EventResolver& m_eventResolver;
};
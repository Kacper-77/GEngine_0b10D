#pragma once

#include "core/ISystem.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include "components/ColliderComponent.h"
#include "event/core/EventBus.h"
#include "event/custom_events/CollisionEvent.h"

#include <functional>

class CollisionSystem : public ISystem {
public:
    CollisionSystem(EntityManager& entityManager,
                    ComponentStorage<ColliderComponent>& colliders,
                    EventBus& eventBus);
    
    void Update(float deltaTime) override;

private:
    bool IsColliding(const ColliderComponent& a, const ColliderComponent& b);
    void HandleCollision(EntityID a, EntityID b);

    EntityManager& m_entityManager;
    ComponentStorage<ColliderComponent>& m_colliders;
    EventBus& m_eventBus;
};
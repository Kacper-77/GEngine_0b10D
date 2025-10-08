#pragma once

#include "core/ISystem.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include "components/ColliderComponent.h"
#include "components/TransformComponent.h"
#include "event/core/EventBus.h"
#include "event/custom_events/CollisionEvent.h"

#include <functional>

class CollisionSystem : public ISystem {
public:
    CollisionSystem(EntityManager& entityManager,
                    ComponentStorage<TransformComponent> & transforms,
                    ComponentStorage<ColliderComponent>& colliders,
                    EventBus& eventBus);
    
    void Update(float deltaTime) override;

private:
    bool IsColliding(int ax, int ay, int aw, int ah,
                     int bx, int by, int bw, int bh);
    void HandleCollision(EntityID a, EntityID b);

    EntityManager& m_entityManager;
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<ColliderComponent>& m_colliders;
    EventBus& m_eventBus;
};
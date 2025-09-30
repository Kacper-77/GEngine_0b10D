#pragma once

#include "core/ISystem.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include "components/ColliderComponent.h"

#include <functional>

using CollisionCallback = std::function<void(EntityID, EntityID)>;

class CollisionSystem : public ISystem {
public:
    CollisionSystem(EntityManager& entityManager,
                    ComponentStorage<ColliderComponent>& colliders);
    
    void Update(float deltaTime) override;
    void SetCollisionCallback(CollisionCallback callback);

private:
    bool IsColliding(const ColliderComponent& a, const ColliderComponent& b);
    void HandleCollision(EntityID a, EntityID b);

    EntityManager& m_entityManager;
    ComponentStorage<ColliderComponent>& m_colliders;
    CollisionCallback m_callback;
};
#pragma once

#include "core/ISystem.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include "components/ColliderComponent.h"

class CollisionSystem : public ISystem {
public:
    CollisionSystem(EntityManager& entityManager,
                    ComponentStorage<ColliderComponent>& colliders);
    
    void Update(float deltaTime) override;

private:
    bool IsColliding(const ColliderComponent& a, const ColliderComponent& b);
    void HandleCollision(EntityID a, EntityID b);

    EntityManager& m_entityManager;
    ComponentStorage<ColliderComponent>& m_colliders;
};
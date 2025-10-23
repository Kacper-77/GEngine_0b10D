#pragma once

#include "core/ISystem.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include "components/ColliderComponent.h"
#include "components/TransformComponent.h"
#include "event/core/EventBus.h"
#include "event/custom_events/CollisionEvent.h"
#include "utils/Int2.h"

// Uniform Spatial Grid
static constexpr int cellSize = 64;

// Hash std::pair<EntityID, EntityID>
struct PairHash {
    std::size_t operator()(const std::pair<EntityID, EntityID>& p) const {
        return std::hash<EntityID>()(p.first) ^ (std::hash<EntityID>()(p.second) << 1);
    }
};

class CollisionSystem : public ISystem {
public:
    CollisionSystem(EntityManager& entityManager,
                    ComponentStorage<TransformComponent> & transforms,
                    ComponentStorage<ColliderComponent>& colliders,
                    EventBus& eventBus);
    
    void Update(float deltaTime) override; // ISystem method

private:
    // Check that entities are colliding
    bool IsColliding(int ax, int ay, int aw, int ah,
                     int bx, int by, int bw, int bh);
    
    // Collision handling between entities
    void CheckAndHandleCollision(EntityID a, EntityID b);

    EntityManager& m_entityManager;
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<ColliderComponent>& m_colliders;
    EventBus& m_eventBus;
    std::unordered_map<Int2, std::vector<EntityID>> m_spatialGrid;
};
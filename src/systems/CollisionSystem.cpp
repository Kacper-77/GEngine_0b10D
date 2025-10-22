#include "systems/CollisionSystem.h"
#include <iostream>

CollisionSystem::CollisionSystem(EntityManager& entityManager,
                                 ComponentStorage<TransformComponent>& transforms,
                                 ComponentStorage<ColliderComponent>& colliders,
                                 ComponentStorage<PhysicsComponent>& physics,
                                 EventBus& eventBus)
        : m_entityManager{entityManager}, 
          m_transforms{transforms}, 
          m_colliders{colliders},
          m_physics{physics}, 
          m_eventBus{eventBus} {}

// Update state
void CollisionSystem::Update(float deltaTime) {
    m_spatialGrid.clear();

    // Insert to grid
    for (EntityID id : m_entityManager.GetAllEntities()) {
        if (!m_transforms.Has(id) || !m_colliders.Has(id)) continue;

        const auto* t = m_transforms.Get(id);
        const auto* c = m_colliders.Get(id);

        int startX = t->x / cellSize;
        int endX = (t->x + c->width) / cellSize;
        int startY = t->y / cellSize;
        int endY = (t->y + c->height) / cellSize;

        for (int x = startX; x <= endX; ++x) {
            for (int y = startY; y <= endY; ++y) {
                m_spatialGrid[{x, y}].push_back(id);
            }
        }
    }

    // Check collisions
    std::unordered_set<std::pair<EntityID, EntityID>, PairHash> checked;

    for (const auto& [cell, entities] : m_spatialGrid) {
        for (EntityID a : entities) {
            for (EntityID b : entities) {
                if (a >= b || checked.count({a, b})) continue;
                checked.insert({a, b});
                CheckAndHandleCollision(a, b);
            }

            // Check neighbors
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    Int2 neighbor = {cell.x + dx, cell.y + dy};
                    auto it = m_spatialGrid.find(neighbor);
                    if (it == m_spatialGrid.end()) continue;

                    for (EntityID b : it->second) {
                        if (a >= b || checked.count({a, b})) continue;
                        checked.insert({a, b});
                        CheckAndHandleCollision(a, b);
                    }
                }
            }
        }
    }
}

// Check collision between entities
bool CollisionSystem::IsColliding(int ax, int ay, int aw, int ah,
                                  int bx, int by, int bw, int bh) {
    return !(ax + aw <= bx ||
             ax >= bx + bw ||
             ay + ah <= by ||
             ay >= by + bh);
}

// Handle collision and publish event
void CollisionSystem::CheckAndHandleCollision(EntityID a, EntityID b) {
    const auto& ta = *m_transforms.Get(a);
    const auto& tb = *m_transforms.Get(b);
    const auto& ca = *m_colliders.Get(a);
    const auto& cb = *m_colliders.Get(b);

    if (IsColliding(ta.x, ta.y, ca.width, ca.height,
                    tb.x, tb.y, cb.width, cb.height)) {
        // Get info about entities
        std::string typeA = m_entityManager.GetInfo(a, "type");
        std::string typeB = m_entityManager.GetInfo(b, "type");

        m_eventBus.PublishImmediate(CollisionEvent{a, b, typeA, typeB});
    }
}
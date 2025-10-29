#include "systems/CollisionSystem.h"
#include <iostream>

CollisionSystem::CollisionSystem(EntityManager& entityManager,
                                 ComponentStorage<TransformComponent>& transforms,
                                 ComponentStorage<ColliderComponent>& colliders,
                                 EventBus& eventBus)
        : m_entityManager{entityManager}, 
          m_transforms{transforms}, 
          m_colliders{colliders}, 
          m_eventBus{eventBus} {}

// Update state
void CollisionSystem::Update(float deltaTime) {
    const int cellSize = m_spatialGrid.GetCellSize();
    m_spatialGrid.Clear();

    // Insert to grid
    for (EntityID id : m_entityManager.GetAllEntities()) {
        if (!m_transforms.Has(id) || !m_colliders.Has(id)) continue;

        const auto* t = m_transforms.Get(id);
        const auto* c = m_colliders.Get(id);

        // Nessesary to divide here! We want to add cells not pixels to grid
        const int startX = t->x / cellSize;
        const int endX = (t->x + c->width) / cellSize;
        const int startY = t->y / cellSize;
        const int endY = (t->y + c->height) / cellSize;

        for (int x = startX; x <= endX; ++x) {
            for (int y = startY; y <= endY; ++y) {
                m_spatialGrid.Insert({x, y}, id);
            }
        }
    }

    // Checked collisions
    std::unordered_set<std::pair<EntityID, EntityID>, PairHash> checked;

    // Getting all cells with entities inside
    for (const auto& [cell, entities] : m_spatialGrid.GetAllCells()) {
        for (EntityID a : entities) {
            for (EntityID b : entities) {
                auto pair = (a < b) ? std::make_pair(a, b) : std::make_pair(b, a);
                if (checked.count(pair)) continue;
                checked.insert(pair);
                CheckAndHandleCollision(a, b);
            }
            // Get neighbors
            auto neighbors = m_spatialGrid.QueryNeighbors(cell.x, cell.y);

            // Check and handle neighbors
            for (EntityID b : neighbors) {
                auto pair = (a < b) ? std::make_pair(a, b) : std::make_pair(b, a);
                if (checked.count(pair)) continue;
                checked.insert(pair);
                CheckAndHandleCollision(a, b);
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
                        //  std::cout << "COLLISION DETECTED!\n";  // TO ERASE LATER
        m_eventBus.PublishImmediate(CollisionEvent{a, b, typeA, typeB});
    }
}
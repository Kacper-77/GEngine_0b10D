#include "systems/CollisionSystem.h"
#include <iostream>

CollisionSystem::CollisionSystem(EntityManager& entityManager,
                                 ComponentStorage<TransformComponent>& transforms,
                                 ComponentStorage<ColliderComponent>& colliders)
        : m_entityManager{entityManager}, 
          m_transforms{transforms}, 
          m_colliders{colliders} {}

void CollisionSystem::Update(float deltaTime) {
    const int cellSize = m_spatialGrid.GetCellSize();
    m_spatialGrid.Clear();
    m_collisions.clear();

    // Insert to grid
    for (EntityID id : m_entityManager.GetAllEntities()) {
        if (!m_transforms.Has(id) || !m_colliders.Has(id)) continue;

        const auto* t = m_transforms.Get(id);
        const auto* c = m_colliders.Get(id);

        const float x = t->position.x;
        const float y = t->position.y;
        const int w = c->width;
        const int h = c->height;

        const int startX = static_cast<int>(std::floor(x / cellSize));
        const int endX   = static_cast<int>(std::floor((x + w) / cellSize));
        const int startY = static_cast<int>(std::floor(y / cellSize));
        const int endY   = static_cast<int>(std::floor((y + h) / cellSize));

        for (int cx = startX; cx <= endX; ++cx) {
            for (int cy = startY; cy <= endY; ++cy) {
                m_spatialGrid.Insert({cx, cy}, id);
            }
        }
    }

    // Check collisions in all occupied cells
    std::unordered_set<std::pair<EntityID, EntityID>, PairHash> checked;
    for (const auto& [cell, entities] : m_spatialGrid.GetAllCells()) {
        for (EntityID a : entities) {
            for (EntityID b : entities) {
                if (a == b) continue;
                auto pair = (a < b) ? std::make_pair(a, b) : std::make_pair(b, a);
                if (checked.count(pair)) continue;
                checked.insert(pair);

                CheckAndHandleCollision(a, b);
            }
            auto neighbors = m_spatialGrid.QueryNeighbors(cell.x, cell.y);
            for (EntityID b : neighbors) {
                if (a == b) continue;
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
    const auto* ta = m_transforms.Get(a);
    const auto* tb = m_transforms.Get(b);
    const auto* ca = m_colliders.Get(a);
    const auto* cb = m_colliders.Get(b);

    if (!ta || !tb || !ca || !cb) return;

    // Skip if entities cannot collide
    if (!CanCollide(*ca, *cb)) return;

    const float ax = ta->position.x;
    const float ay = ta->position.y;
    const int   aw = ca->width;
    const int   ah = ca->height;

    const float bx = tb->position.x;
    const float by = tb->position.y;
    const int   bw = cb->width;
    const int   bh = cb->height;

    if (IsColliding(ax, ay, aw, ah, bx, by, bw, bh)) {
        m_collisions.push_back( {a, b} );
    }
}

// Get all collisions
const std::vector<std::pair<EntityID, EntityID>>& CollisionSystem::GetCollisions() const {
    return m_collisions;
}

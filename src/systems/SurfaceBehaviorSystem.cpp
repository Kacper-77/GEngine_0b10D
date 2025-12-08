#include "systems/SurfaceBehaviorSystem.h"

SurfaceBehaviorSystem::SurfaceBehaviorSystem(
    ComponentStorage<TransformComponent>& transforms,
    ComponentStorage<VelocityComponent>& velocities,
    ComponentStorage<SurfaceComponent>& surfaces,
    ComponentStorage<PhysicsComponent>& physics,
    SpatialGrid<EntityID>& spatialGrid)
    : m_transforms{transforms},
      m_velocities{velocities},
      m_surfaces{surfaces},
      m_physics{physics},
      m_spatialGrid{spatialGrid} {}

void SurfaceBehaviorSystem::Update(float deltaTime) {
    const int cellSize = m_spatialGrid.GetCellSize();
    m_spatialGrid.Clear();

    // Insert surfaces into spatial grid
    for (const auto& [surfaceID, _] : m_surfaces.GetAll()) {
        const auto* t = m_transforms.Get(surfaceID);
        if (!t) continue;

        float x = t->position.x;
        float y = t->position.y;
        float w = t->scale.x;
        float h = t->scale.y;

        int startX = x / cellSize;
        int endX   = (x + w) / cellSize;
        int startY = y / cellSize;
        int endY   = (y + h) / cellSize;

        for (int cx = startX; cx <= endX; ++cx)
            for (int cy = startY; cy <= endY; ++cy)
                m_spatialGrid.Insert({cx, cy}, surfaceID);
    }

    // Apply surface behavior
    for (const auto& [entityID, _] : m_transforms.GetAll()) {
        const auto* t = m_transforms.Get(entityID);
        if (!t) continue;

        float ex = t->position.x;
        float ey = t->position.y;

        int cellX = ex / cellSize;
        int cellY = ey / cellSize;

        auto nearby = m_spatialGrid.Query(cellX, cellY);
        auto neighbors = m_spatialGrid.QueryNeighbors(cellX, cellY);
        nearby.insert(nearby.end(), neighbors.begin(), neighbors.end());

        for (EntityID surfaceID : nearby) {
            const auto* surface = m_surfaces.Get(surfaceID);
            const auto* st = m_transforms.Get(surfaceID);
            if (!surface || !st) continue;

            float sx = st->position.x - st->scale.x * 0.5f;
            float sy = st->position.y - st->scale.y * 0.5f;
            float sw = st->scale.x;
            float sh = st->scale.y;

            bool inside =
                ex >= sx && ex <= sx + sw &&
                ey >= sy && ey <= sy + sh;

            if (!inside) continue;

            if (auto* vel = m_velocities.Get(entityID)) {
                vel->dx *= surface->multiplier;
                vel->dy *= surface->multiplier;
            }

            if (auto* phys = m_physics.Get(entityID)) {
                phys->velocity.x *= surface->multiplier;
                phys->velocity.y *= surface->multiplier;

                // Change friction
                phys->frictionKinetic *= surface->frictionMultiplier;
            }

            break;
        }
    }
}


void SurfaceBehaviorSystem::SetVelocityBySurfaceType(SurfaceType type, float velocity) {
    m_surfaceVelocity[type] = velocity;
}

void SurfaceBehaviorSystem::SetDefaultVelocities() {
    m_surfaceVelocity[SurfaceType::GRASS]  = 0.8f;
    m_surfaceVelocity[SurfaceType::ICE]    = 1.05f;
    m_surfaceVelocity[SurfaceType::SAND]   = 0.4f;
    m_surfaceVelocity[SurfaceType::LIQUID] = 0.2f;
}

float SurfaceBehaviorSystem::GetVelocityBySurfaceType(SurfaceType type) const {
    const auto it = m_surfaceVelocity.find(type);
    return it != m_surfaceVelocity.end() ? it->second : 1.0f;
}

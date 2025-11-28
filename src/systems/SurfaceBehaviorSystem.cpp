#include "systems/SurfaceBehaviorSystem.h"

SurfaceBehaviorSystem::SurfaceBehaviorSystem(ComponentStorage<TransformComponent>& transforms,
                                             ComponentStorage<VelocityComponent>& velocities,
                                             ComponentStorage<SurfaceComponent>& surfaces,
                                             SpatialGrid<EntityID>& spatialGrid)
    : m_transforms{transforms},
      m_velocities{velocities},
      m_surfaces{surfaces},
      m_spatialGrid{spatialGrid} {}

// Update state
void SurfaceBehaviorSystem::Update(float deltaTime) {
    const int cellSize = m_spatialGrid.GetCellSize();
    m_spatialGrid.Clear();

    // Insert surfaces into spatial grid
    for (const auto& [surfaceID, _] : m_surfaces.GetAll()) {
        const auto* surface = m_surfaces.Get(surfaceID);
        if (!surface) continue;

        const int startX = surface->x / cellSize;
        const int endX = (surface->x + surface->width) / cellSize;
        const int startY = surface->y / cellSize;
        const int endY = (surface->y + surface->height) / cellSize;

        for (int x = startX; x <= endX; ++x) {
            for (int y = startY; y <= endY; ++y) {
                m_spatialGrid.Insert({x, y}, surfaceID);
            }
        }
    }

    // Apply surface behavior to entities
    for (const auto& [entityID, _] : m_transforms.GetAll()) {
        const auto* transform = m_transforms.Get(entityID);
        if (!transform || !m_velocities.Has(entityID)) continue;

        auto* velocity = m_velocities.Get(entityID);
        int cellX = transform->position.x / cellSize;
        int cellY = transform->position.y / cellSize;

        std::vector<EntityID> nearbySurfaces = m_spatialGrid.Query(cellX, cellY);
        std::vector<EntityID> neighbors = m_spatialGrid.QueryNeighbors(cellX, cellY);
        nearbySurfaces.insert(nearbySurfaces.end(), neighbors.begin(), neighbors.end());

        for (EntityID surfaceID : nearbySurfaces) {
            const auto* surface = m_surfaces.Get(surfaceID);
            if (!surface) continue;

            if (surface->Contains(transform->position.x, transform->position.y)) {
                float multiplier = GetVelocityBySurfaceType(surface->surfaceType);

                // Set velocity by type
                velocity->dx *= multiplier;
                velocity->dy *= multiplier;
            }
                break; // Only one surface applies
        }
    }
}

void SurfaceBehaviorSystem::SetVelocityBySurfaceType(SurfaceType type, float velocity) {
    m_surfaceVelocity[type] = velocity;
}

void SurfaceBehaviorSystem::SetDefaultVelocities() {
    m_surfaceVelocity[SurfaceType::GRASS] = 0.8f;
    m_surfaceVelocity[SurfaceType::ICE] = 1.05f;
    m_surfaceVelocity[SurfaceType::SAND] = 0.4f;
    m_surfaceVelocity[SurfaceType::LIQUID] = 0.2f;
}

float SurfaceBehaviorSystem::GetVelocityBySurfaceType(SurfaceType type) const {
    const auto it = m_surfaceVelocity.find(type);

    // If velocity is not set then return const 1.0f by
    return it != m_surfaceVelocity.end() ? it->second : 1.0f;
}

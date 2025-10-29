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
        int cellX = transform->x / cellSize;
        int cellY = transform->y / cellSize;

        std::vector<EntityID> nearbySurfaces = m_spatialGrid.Query(cellX, cellY);
        std::vector<EntityID> neighbors = m_spatialGrid.QueryNeighbors(cellX, cellY);
        nearbySurfaces.insert(nearbySurfaces.end(), neighbors.begin(), neighbors.end());

        for (EntityID surfaceID : nearbySurfaces) {
            const auto* surface = m_surfaces.Get(surfaceID);
            if (!surface) continue;

            if (surface->Contains(transform->x, transform->y)) {
                switch (surface->surfaceType) {
                    case SurfaceType::ICE:
                        velocity->dx *= 1.05f;
                        velocity->dy *= 1.05f;
                        break;
                    case SurfaceType::SAND:
                        velocity->dx *= 0.6f;
                        velocity->dy *= 0.6f;
                        break;
                    case SurfaceType::GRASS:
                        velocity->dx *= 0.9f;
                        velocity->dy *= 0.9f;
                        break;
                    case SurfaceType::LIQUID:
                        velocity->dx *= 0.4f;
                        velocity->dy *= 0.4f;
                        break;
                    case SurfaceType::CUSTOM:
                        break;
                }
                break; // Only one surface applies
            }
        }
    }
}

#include "systems/SurfaceBehaviorSystem.h"

SurfaceBehaviorSystem::SurfaceBehaviorSystem(ComponentStorage<TransformComponent>& transforms,
                                             ComponentStorage<VelocityComponent>& velocities,
                                             ComponentStorage<SurfaceComponent>& surfaces)
    : m_transforms(transforms),
      m_velocities(velocities),
      m_surfaces(surfaces) {}

// Update state
void SurfaceBehaviorSystem::Update(float deltaTime) {
    // Get transform for all entities
    for (auto& [entityID, transform] : m_transforms.GetAll()) {
        if (!m_velocities.Has(entityID)) continue;

        auto* velocity = m_velocities.Get(entityID);
        
        // Check surface type and handle it
        for (auto& [surfaceID, surface] : m_surfaces.GetAll()) {
            if (surface.Contains(transform.x, transform.y)) {
                switch (surface.surfaceType) {
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
                break;  // Entity can only be on one surface at a time
            }
        }
    }
}

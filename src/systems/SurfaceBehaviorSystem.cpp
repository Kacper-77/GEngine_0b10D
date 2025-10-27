#include "systems/SurfaceBehaviorSystem.h"

SurfaceBehaviorSystem::SurfaceBehaviorSystem(ComponentStorage<TransformComponent>& transforms,
                                             ComponentStorage<VelocityComponent>& velocities,
                                             ComponentStorage<SurfaceComponent>& surfaces)
    : m_transforms(transforms),
      m_velocities(velocities),
      m_surfaces(surfaces) {}

// Update state
void SurfaceBehaviorSystem::Update(float deltaTime) {

}

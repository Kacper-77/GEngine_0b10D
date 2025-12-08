#pragma once

#include <unordered_map>

#include "utils/SpatialGrid.h"
#include "core/ISystem.h"
#include "event/core/EventBus.h"
#include "core/ComponentStorage.h"

#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "components/SurfaceComponent.h"
#include "components/PhysicsComponent.h"

class SurfaceBehaviorSystem : public ISystem {
public:
    SurfaceBehaviorSystem(ComponentStorage<TransformComponent>& transforms,
                          ComponentStorage<VelocityComponent>& velocities,
                          ComponentStorage<SurfaceComponent>& surfaces,
                          ComponentStorage<PhysicsComponent>& physics,
                          SpatialGrid<EntityID>& spatialGrid);
    
    void Update(float deltaTime) override;  // ISystem method

    void SetVelocityBySurfaceType(SurfaceType type, float velocity);
    void SetDefaultVelocities();

private:
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<VelocityComponent>& m_velocities;
    ComponentStorage<SurfaceComponent>& m_surfaces;
    ComponentStorage<PhysicsComponent>& m_physics;
    SpatialGrid<EntityID>& m_spatialGrid;
    
    float GetVelocityBySurfaceType(SurfaceType type) const;

    std::unordered_map<SurfaceType, float> m_surfaceVelocity;
};

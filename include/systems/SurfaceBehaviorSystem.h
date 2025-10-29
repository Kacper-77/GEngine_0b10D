#pragma once

#include "utils/SpatialGrid.h"
#include "core/ISystem.h"
#include "event/core/EventBus.h"
#include "core/ComponentStorage.h"

#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "components/SurfaceComponent.h"

class SurfaceBehaviorSystem : public ISystem {
public:
    SurfaceBehaviorSystem(ComponentStorage<TransformComponent>& transforms,
                          ComponentStorage<VelocityComponent>& velocities,
                          ComponentStorage<SurfaceComponent>& surfaces,
                          SpatialGrid<EntityID>& spatialGrid);
    
    void Update(float deltaTime) override;  // ISystem method

private:
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<VelocityComponent>& m_velocities;
    ComponentStorage<SurfaceComponent>& m_surfaces;
    SpatialGrid<EntityID>& m_spatialGrid;
};

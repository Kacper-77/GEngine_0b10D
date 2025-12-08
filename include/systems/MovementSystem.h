#pragma once

#include "core/ISystem.h"
#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "components/AccelerationComponent.h"
#include "components/PhysicsComponent.h"

class MovementSystem : public ISystem {
public:
    MovementSystem(ComponentStorage<TransformComponent>& transforms,
                   ComponentStorage<VelocityComponent>& velocities,
                   ComponentStorage<AccelerationComponent>& accelerations,
                   ComponentStorage<PhysicsComponent>& physics);

    void Update(float deltaTime) override;  // ISystem method

private:
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<VelocityComponent>& m_velocities;
    ComponentStorage<AccelerationComponent>& m_accelerations;
    ComponentStorage<PhysicsComponent>& m_physics;
};

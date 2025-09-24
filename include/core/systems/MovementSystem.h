#pragma once

#include "core/ISystem.h"
#include "core/ComponentStorage.h"
#include "core/components/TransformComponent.h"
#include "core/components/VelocityComponent.h"
#include "core/components/AccelerationComponent.h"

class MovementSystem : public ISystem {
public:
    MovementSystem(ComponentStorage<TransformComponent>& transforms,
                   ComponentStorage<VelocityComponent>& velocities,
                   ComponentStorage<AccelerationComponent>& accelerations);

    void Update(float deltaTime) override;

private:
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<VelocityComponent>& m_velocities;
    ComponentStorage<AccelerationComponent>& m_accelerations;
};

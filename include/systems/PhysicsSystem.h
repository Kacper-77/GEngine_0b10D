#pragma once

#include "core/ISystem.h"
#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"
#include "components/PhysicsComponent.h"
#include "components/AccelerationComponent.h"

class PhysicsSystem : public ISystem {
public:
    PhysicsSystem(ComponentStorage<TransformComponent>& transforms,
                  ComponentStorage<AccelerationComponent>& accelerations,
                  ComponentStorage<PhysicsComponent>& physics);

    void Update(float deltaTime) override;  // ISystem method

private:
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<PhysicsComponent>& m_physics;
    ComponentStorage<AccelerationComponent>& m_accelerations;
};

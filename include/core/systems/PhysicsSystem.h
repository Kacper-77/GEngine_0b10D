#pragma once

#include "core/ISystem.h"
#include "core/ComponentStorage.h"
#include "core/components/TransformComponent.h"
#include "core/components/PhysicsComponent.h"
#include "core/components/AccelerationComponent.h"

class PhysicsSystem : public ISystem {
public:
    PhysicsSystem(ComponentStorage<TransformComponent>& transforms,
                  ComponentStorage<AccelerationComponent>& accelerations,
                  ComponentStorage<PhysicsComponent>& physics);

    void Update(float deltaTime) override;
private:
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<PhysicsComponent>& m_physics;
    ComponentStorage<AccelerationComponent>& m_accelerations;
};
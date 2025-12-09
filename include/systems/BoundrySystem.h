#pragma once

#include "core/ISystem.h"
#include "window/Window.h"
#include "core/ComponentStorage.h"
#include "components/BoundryComponent.h"
#include "components/TransformComponent.h"
#include "components/PhysicsComponent.h"

class BoundrySystem : public ISystem {
public:
    BoundrySystem(ComponentStorage<TransformComponent>& transforms,
                  ComponentStorage<BoundryComponent>& boundaries,
                  ComponentStorage<PhysicsComponent>& physics,
                  Window* window);

    void Update(float deltaTime) override;

private:
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<BoundryComponent>& m_boundaries;
    ComponentStorage<PhysicsComponent>& m_physics;
    Window* m_window;
};
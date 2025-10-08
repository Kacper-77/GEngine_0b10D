#pragma once

#include "core/ISystem.h"
#include "window/Window.h"
#include "core/ComponentStorage.h"
#include "components/BoundryComponent.h"
#include "components/TransformComponent.h"

class BoundrySystem : public ISystem {
public:
    BoundrySystem(ComponentStorage<TransformComponent>& transforms,
                  ComponentStorage<BoundryComponent>& boundries,
                  Window* window);

    void Update(float deltaTime) override;

private:
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<BoundryComponent>& m_boundries;
    Window* m_window;
};
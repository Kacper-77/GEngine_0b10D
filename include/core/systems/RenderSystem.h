#pragma once

#include "core/ISystem.h"
#include "core/ComponentStorage.h"
#include "core/components/TransformComponent.h"
#include "scene/SpriteComponent.h"
#include "graphics/Renderer.h"

class RenderSystem : public ISystem {
public:
    RenderSystem(ComponentStorage<TransformComponent>& transforms,
                 ComponentStorage<SpriteComponent>& sprites,
                 Renderer* renderer);
    
    void Update(float deltaTime) override;
private:
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<SpriteComponent>& m_sprites;
    Renderer* m_renderer;
};
#pragma once

#include "core/ISystem.h"
#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"
#include "scene/SpriteComponent.h"
#include "graphics/Renderer.h"
#include "core/IRenderer.h"

class RenderSystem : public ISystem {
public:
    RenderSystem(ComponentStorage<TransformComponent>& transforms,
                 ComponentStorage<SpriteComponent>& sprites,
                 IRenderer* renderer);
    
    void Update(float deltaTime) override;
private:
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<SpriteComponent>& m_sprites;
    IRenderer* m_renderer;
};
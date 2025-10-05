#include "systems/RenderSystem.h"

RenderSystem::RenderSystem(ComponentStorage<TransformComponent>& transforms,
                           ComponentStorage<SpriteComponent>& sprites,
                           IRenderer* renderer) 
    :   m_transforms(transforms), m_sprites(sprites), m_renderer(renderer) {}

void RenderSystem::Update(float deltaTime) {
    for (auto& [id, sprite] : m_sprites.GetAll()) {
        auto* transform = m_transforms.Get(id);
        if (!transform || !sprite.texture) continue;

        SDL_Rect dstRect = {
            transform->x,
            transform->y,
            sprite.width,
            sprite.height
        };
        m_renderer->DrawTexture(sprite.texture->GetSDLTexture(), nullptr, &dstRect);
    }
}

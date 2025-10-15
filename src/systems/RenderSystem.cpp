#include "systems/RenderSystem.h"
#include <iostream>

RenderSystem::RenderSystem(ComponentStorage<TransformComponent>& transforms,
                           ComponentStorage<SpriteComponent>& sprites,
                           IRenderer* renderer) 
    :   m_transforms{transforms}, m_sprites{sprites}, m_renderer{renderer},
    m_cameraZoom{1.0f}, m_rotationDegrees{0.0f}, m_fadeAlpha{255}, m_cameraPosition{0, 0}, m_viewport{800, 600} {}

void RenderSystem::Update(float deltaTime) {
    for (auto& [id, sprite] : m_sprites.GetAll()) {
        auto* transform = m_transforms.Get(id);
        if (!transform || !sprite.texture) continue;

        SDL_Rect dstRect = {
            static_cast<int>((transform->x - m_cameraPosition.x) * m_cameraZoom),
            static_cast<int>((transform->y - m_cameraPosition.y) * m_cameraZoom),
            static_cast<int>(sprite.width * m_cameraZoom),
            static_cast<int>(sprite.height * m_cameraZoom)
        };
        m_renderer->DrawTexture(sprite.texture->GetSDLTexture(), nullptr, &dstRect);
    }
}

void RenderSystem::SetCameraPosition(const SDL_Point& position) {
    m_cameraPosition = position;
}

void RenderSystem::SetCameraZoom(float zoom) {
    m_cameraZoom = zoom;
}

void RenderSystem::SetCameraRotation(float degrees) {
    m_rotationDegrees = degrees;
}

void RenderSystem::SetFadeAlpha(Uint8 fadeAlpha) {
    m_fadeAlpha = fadeAlpha;
}

void RenderSystem::SetViewportSize(SDL_Point size) {
    m_viewport = size;
}

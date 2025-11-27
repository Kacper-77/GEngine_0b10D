#include "systems/RenderSystem.h"
#include <iostream>

RenderSystem::RenderSystem(ComponentStorage<TransformComponent>& transforms,
                           ComponentStorage<SpriteComponent>& sprites,
                           IRenderer* renderer) 
    :   m_transforms{transforms}, m_sprites{sprites}, m_renderer{renderer} {}

// Update state
void RenderSystem::Update(float deltaTime) {
    // Set backround
    DrawBackgroundLayers();

    // Main rendering loop
    for (auto& [id, sprite] : m_sprites.GetAll()) {
        auto* transform = m_transforms.Get(id);
        if (!transform || !sprite.texture) continue;

        SDL_Rect dstRect = {
            static_cast<int>((transform->position.x - m_cameraPosition.x) * m_cameraZoom),
            static_cast<int>((transform->position.y - m_cameraPosition.y) * m_cameraZoom),
            static_cast<int>(sprite.width * m_cameraZoom),
            static_cast<int>(sprite.height * m_cameraZoom)
        };
        // DrawTexture from Renderer.cpp
        m_renderer->DrawTexture(sprite.texture->GetSDLTexture(), nullptr, &dstRect);
    }
}

// Background methods
void RenderSystem::AddBackgroundLayer(Texture* texture, float parallaxFactor) {
    m_backgroundLayers.push_back({texture, parallaxFactor});
}
void RenderSystem::ClearBackgroundLayers() {
    m_backgroundLayers.clear();
}

void RenderSystem::DrawBackgroundLayers() {
    for (auto& layer : m_backgroundLayers) {
        if (!layer.texture) continue;

        int texW, texH;
        SDL_QueryTexture(layer.texture->GetSDLTexture(), nullptr, nullptr, &texW, &texH);

        // Repeat enough times to fill screen
        for (int y = -texH; y < m_viewport.y + texH; y += texH) {
            for (int x = -texW; x < m_viewport.x + texW; x += texW) {
                // No static cast to avoid signed values (%)
                int offsetX = ((int)(x - m_cameraPosition.x * layer.parallaxFactor) % texW + texW) % texW;
                int offsetY = ((int)(y - m_cameraPosition.y * layer.parallaxFactor) % texH + texH) % texH;

                SDL_Rect dstRect = { offsetX, offsetY, texW, texH };
                m_renderer->DrawTexture(layer.texture->GetSDLTexture(), nullptr, &dstRect);
            }
        }
    }
}

// Setters
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

// Getter
const SDL_Point& RenderSystem::GetCameraPosition() const {
    return m_cameraPosition;
}

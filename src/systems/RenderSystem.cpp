#include "systems/RenderSystem.h"
#include <iostream>

RenderSystem::RenderSystem(ComponentStorage<TransformComponent>& transforms,
                           ComponentStorage<SpriteComponent>& sprites,
                           IRenderer* renderer) 
    : m_transforms{transforms}, m_sprites{sprites}, m_renderer{renderer} {}

// Update state
void RenderSystem::Update(float deltaTime) {
    if (m_viewport.x > 0 && m_viewport.y > 0) {
        DrawBackgroundLayers();
    }

    for (auto& [id, sprite] : m_sprites.GetAll()) {
        auto* transform = m_transforms.Get(id);
        if (!transform || !sprite.texture) continue;

        SDL_Rect dstRect = {
            static_cast<int>((transform->position.x - sprite.width * 0.5f - m_cameraPosition.x) * m_cameraZoom),
            static_cast<int>((transform->position.y - sprite.height * 0.5f - m_cameraPosition.y) * m_cameraZoom),
            static_cast<int>(sprite.width * m_cameraZoom),
            static_cast<int>(sprite.height * m_cameraZoom)
        };

        m_renderer->DrawTexture(sprite.texture->GetSDLTexture(), nullptr, &dstRect);
    }
}

void RenderSystem::AddBackgroundLayer(Texture* texture, float parallaxFactor) {
    if (!texture) {
        std::cerr << "RenderSystem::AddBackgroundLayer: null texture\n";
        return;
    }

    int w = 0, h = 0;
    if (SDL_QueryTexture(texture->GetSDLTexture(), nullptr, nullptr, &w, &h) != 0) {
        std::cerr << "RenderSystem::AddBackgroundLayer: SDL_QueryTexture failed: "
                  << SDL_GetError() << "\n";
        return;
    }

    BackgroundLayer layer;
    layer.texture = texture;
    layer.parallaxFactor = parallaxFactor;
    layer.texW = w;
    layer.texH = h;

    m_backgroundLayers.push_back(layer);
}

void RenderSystem::ClearBackgroundLayers() {
    m_backgroundLayers.clear();
}

void RenderSystem::DrawBackgroundLayers() {
    for (auto& layer : m_backgroundLayers) {
        if (!layer.texture) continue;

        const int texW = layer.texW;
        const int texH = layer.texH;

        if (texW <= 0 || texH <= 0) {
            continue;
        }

        // Calculate parallax
        const float camX = static_cast<float>(m_cameraPosition.x) * layer.parallaxFactor;
        const float camY = static_cast<float>(m_cameraPosition.y) * layer.parallaxFactor;

        // Start offset
        int baseOffsetX = static_cast<int>(-camX);
        int baseOffsetY = static_cast<int>(-camY);

        // Normalize
        baseOffsetX %= texW;
        baseOffsetY %= texH;
        if (baseOffsetX > 0) baseOffsetX -= texW;
        if (baseOffsetY > 0) baseOffsetY -= texH;

        // Cover whole veiwport
        for (int y = baseOffsetY; y < m_viewport.y; y += texH) {
            for (int x = baseOffsetX; x < m_viewport.x; x += texW) {
                SDL_Rect dstRect{ x, y, texW, texH };
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

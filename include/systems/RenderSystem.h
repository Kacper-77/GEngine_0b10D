#pragma once

#include "core/ISystem.h"
#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"
#include "components/SpriteComponent.h"
#include "graphics/Renderer.h"
#include "core/IRenderer.h"

class RenderSystem : public ISystem {
public:
    RenderSystem(ComponentStorage<TransformComponent>& transforms,
                 ComponentStorage<SpriteComponent>& sprites,
                 IRenderer* renderer);
    
    void Update(float deltaTime) override;  // ISystem method

    // Set and get camera position
    void SetCameraPosition(const SDL_Point& position);
    const SDL_Point& GetCameraPosition() const;

    // Other setters
    void SetCameraZoom(float zoom);
    void SetCameraRotation(float degrees);
    void SetFadeAlpha(Uint8 alpha);
    void SetViewportSize(SDL_Point size);

private:
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<SpriteComponent>& m_sprites;
    SDL_Point m_cameraPosition = {0, 0};

    float m_cameraZoom;
    float m_rotationDegrees;
    Uint8 m_fadeAlpha;
    SDL_Point m_viewport;
    IRenderer* m_renderer;
};
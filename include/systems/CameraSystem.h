#pragma once

#include "core/ISystem.h"
#include "event/core/EventBus.h"
#include "core/ComponentStorage.h"
#include "systems/RenderSystem.h"
#include "components/TransformComponent.h"
#include "components/CameraComponent.h"

class CameraSystem : public ISystem {
public:
    CameraSystem(ComponentStorage<TransformComponent>& transforms,
                 ComponentStorage<CameraComponent>& camera);

    void Update(float deltaTime) override; // ISystem method

    void FocusOn(EntityID target);
    void MoveTo(SDL_Point target);
    void SetZoom(std::optional<EntityID> cameraEntity, float zoom);
    void SetShake(int intensity, float duration);
    void SetActiveCamera(EntityID cameraEntity);
    std::optional<EntityID> GetActiveCamera() const;
    
private:
    ComponentStorage<TransformComponent>& m_transforms;
    ComponentStorage<CameraComponent>& m_camera;
    EventBus* m_eventBus;

    std::optional<EntityID> m_activeCamera; // current camera
    CameraComponent* CheckActiveCamera();  // helper to avoid boilerplate

    void ApplyToRenderSystem(RenderSystem& renderSystem);

    // helpers
    void UpdateCameraPosition(CameraComponent& cam, float deltaTime);
    void ApplyShake(CameraComponent& cam);
    void ClampToBounds(CameraComponent& cam);
};
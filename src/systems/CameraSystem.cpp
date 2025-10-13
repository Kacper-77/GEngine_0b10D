#include "systems/CameraSystem.h"

CameraSystem::CameraSystem(ComponentStorage<TransformComponent>& transforms,
                           ComponentStorage<CameraComponent>& camera)
    : m_transforms{transforms}, m_camera{camera} {}

void CameraSystem::Update(float deltaTime) {

}

void CameraSystem::FocusOn(EntityID target) {
    if (auto* cam = CheckActiveCamera())
    cam->target = target;
}

void CameraSystem::MoveTo(SDL_Point target) {
    if (auto* cam = CheckActiveCamera()) {
        cam->position = target;
        cam->manualControl = true;
        cam->mode = CameraMode::Static;
        cam->target = INVALID_ENTITY;
    }
}

void CameraSystem::SetZoom(std::optional<EntityID> cameraEntity, float zoom) {
    if (auto* cam = CheckActiveCamera()) {
        zoom = std::clamp(zoom, 0.1f, 5.0f);
        cam->zoom = zoom;

        if (cam->target != INVALID_ENTITY) {
            cam->mode = CameraMode::Follow;
            cam->manualControl = false;
        } else {
            cam->mode = CameraMode::Static;
            cam->manualControl = true;
        }
    }
}

void CameraSystem::SetShake(int intensity, float duration) {
    if (auto* cam = CheckActiveCamera()) {
        intensity = std::clamp(intensity, 0, 15);
        duration = std::clamp(duration, 0.0f, 900.0f);

        cam->shakeIntensity = intensity;
        cam->shakeDuration = duration;
        cam->mode = CameraMode::Shake;
    }
}

void CameraSystem::SetActiveCamera(EntityID cameraEntity) {
    m_activeCamera = cameraEntity;
}

std::optional<EntityID> CameraSystem::GetActiveCamera() const {
    return m_activeCamera;
}

void CameraSystem::ApplyToRenderSystem(RenderSystem& renderSystem) {

}

CameraComponent* CameraSystem::CheckActiveCamera() {
    if (!m_activeCamera) return nullptr;
    auto* cam = m_camera.Get(m_activeCamera.value());
    if (!cam || !cam->isActive) return nullptr;

    return cam;
}

void CameraSystem::UpdateCameraPosition(CameraComponent& cam, float deltaTime) {

}

void CameraSystem::ApplyShake(CameraComponent& cam) {

}

void CameraSystem::ClampToBounds(CameraComponent& cam) {

}
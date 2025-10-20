#include "systems/CameraSystem.h"
#include <random>
#include <iostream>

CameraSystem::CameraSystem(ComponentStorage<TransformComponent>& transforms,
                           ComponentStorage<CameraComponent>& camera)
    : m_transforms{transforms}, m_camera{camera} {}

    // Update state
void CameraSystem::Update(float deltaTime) {
    if (auto* cam = CheckActiveCamera()) {
        UpdateCameraPosition(*cam);

        if (cam->shakeDuration > 0) {
            ApplyShake(*cam);
            cam->shakeDuration -= deltaTime;
            if (cam->shakeDuration <= 0) {
                cam->shakeIntensity = 0;
                cam->mode = CameraMode::Static;
            }
        }

        if (cam->useBounds) {
            ClampToBounds(*cam);
        }
    }
}

// Focus on target
void CameraSystem::FocusOn(EntityID target) {
    if (auto* cam = CheckActiveCamera())
    cam->target = target;
}

// Move to target
void CameraSystem::MoveTo(SDL_Point target) {
    if (auto* cam = CheckActiveCamera()) {
        cam->position = target;
        cam->manualControl = true;
        cam->mode = CameraMode::Static;
        cam->target = INVALID_ENTITY;
    }
}

// Setters
void CameraSystem::SetZoom(std::optional<EntityID> cameraEntity, float zoom) {
    if (auto* cam = CheckActiveCamera()) {
        zoom = std::clamp(zoom, 0.1f, 5.0f);  // Set reasonable zoom
        cam->zoom = zoom;

        if (cam->target != INVALID_ENTITY) {  // INVALID_ENTITY is default type declared at include/utils/EntityTypes.h
            cam->mode = CameraMode::Follow;
            cam->manualControl = false;
        } else {
            cam->mode = CameraMode::Static;
            cam->manualControl = true;
        }
    }
}

// Set screen shake
void CameraSystem::SetShake(int intensity, float duration) {
    if (auto* cam = CheckActiveCamera()) {
        // Set reasonable intensuty and duration
        intensity = std::clamp(intensity, 0, 15);
        duration = std::clamp(duration, 0.0f, 900.0f);

        cam->shakeIntensity = intensity;
        cam->shakeDuration = duration;
        cam->mode = CameraMode::Shake;
    }
}

// Set active camera
void CameraSystem::SetActiveCamera(EntityID cameraEntity) {
    if (m_camera.Has(cameraEntity)) {
        m_activeCamera = cameraEntity;
    } else {
        std::cerr << "SetActiveCamera failed: entity " << cameraEntity << " has no CameraComponent\n";
    }
}

// Get active camera
std::optional<EntityID> CameraSystem::GetActiveCamera() const {
    return m_activeCamera;
}

// Send info to RenderSystem
void CameraSystem::ApplyToRenderSystem(RenderSystem& renderSystem) {
    if (auto* cam = CheckActiveCamera()) {
        renderSystem.SetCameraPosition(cam->position);
        renderSystem.SetCameraZoom(cam->zoom);
        renderSystem.SetCameraRotation(cam->rotationDegrees);
        renderSystem.SetFadeAlpha(cam->fadeAlpha);
        renderSystem.SetViewportSize(cam->viewportSize);
    }
}

// Helper to avoid boilerplate
CameraComponent* CameraSystem::CheckActiveCamera() {
    if (!m_activeCamera) return nullptr;
    auto* cam = m_camera.Get(m_activeCamera.value());
    if (!cam || !cam->isActive) return nullptr;

    return cam;
}

// Updating camera position
void CameraSystem::UpdateCameraPosition(CameraComponent& cam) {
    // Check target and mode
    if (cam.manualControl || cam.mode == CameraMode::Static) return;
    if (cam.target == INVALID_ENTITY) return;

    // Get position from transformComponent
    auto* transform = m_transforms.Get(cam.target);
    if (!transform) return;

    // Calculate position
    SDL_Point desired = {
        transform->x + cam.offset.x - cam.viewportSize.x / 2,
        transform->y + cam.offset.y - cam.viewportSize.y / 2
    };

    // Set position
    if (cam.mode == CameraMode::Follow) {
        cam.position = desired;
    } else if (cam.mode == CameraMode::SmoothFollow) {
        cam.position.x += (desired.x - cam.position.x) / cam.smoothing;
        cam.position.y += (desired.y - cam.position.y) / cam.smoothing;
    }
}

// Apply shake
void CameraSystem::ApplyShake(CameraComponent& cam) {
    static std::random_device rd;  // source
    static std::mt19937 generate(rd());  // generation

    std::uniform_int_distribution<int> dist(-cam.shakeIntensity, cam.shakeIntensity);  // range

    int offsetX = dist(generate);
    int offsetY = dist(generate);

    cam.position.x += offsetX;
    cam.position.y += offsetY;
}

// NEED TO THINK ABOUT IT'S FUTURE
void CameraSystem::ClampToBounds(CameraComponent& cam) {
    cam.position.x = std::clamp(cam.position.x, cam.bounds.x, cam.bounds.x + cam.bounds.w - cam.viewportSize.x);
    cam.position.y = std::clamp(cam.position.y, cam.bounds.y, cam.bounds.y + cam.bounds.h - cam.viewportSize.y);
}
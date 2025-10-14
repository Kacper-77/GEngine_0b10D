#include <gtest/gtest.h>
#include "systems/CameraSystem.h"
#include "components/TransformComponent.h"
#include "components/CameraComponent.h"

// Helper to create a basic active camera
CameraComponent CreateActiveCamera() {
    CameraComponent cam;
    cam.isActive = true;
    cam.viewportSize = {800, 600};
    cam.zoom = 1.0f;
    cam.smoothing = 10.0f;
    return cam;
}

TEST(CameraSystemTest, SetActiveCameraStoresCorrectEntity) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<CameraComponent> cameras;
    CameraSystem system(transforms, cameras);

    EntityID camId = 1;
    cameras.Add(camId, CreateActiveCamera());

    system.SetActiveCamera(camId);
    EXPECT_EQ(system.GetActiveCamera().value(), camId);
}

TEST(CameraSystemTest, CheckActiveCameraReturnsValidPointer) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<CameraComponent> cameras;
    CameraSystem system(transforms, cameras);

    EntityID camId = 1;
    cameras.Add(camId, CreateActiveCamera());

    system.SetActiveCamera(camId);
    auto* cam = cameras.Get(camId);
    ASSERT_NE(cam, nullptr);
    EXPECT_TRUE(cam->isActive);
}

TEST(CameraSystemTest, FocusOnUsesActiveCameraCorrectly) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<CameraComponent> cameras;
    CameraSystem system(transforms, cameras);

    EntityID camId = 1;
    CameraComponent cam = CreateActiveCamera();
    cameras.Add(camId, cam);

    system.SetActiveCamera(camId);

    EntityID targetId = 42;
    system.FocusOn(targetId);

    auto* updatedCam = cameras.Get(camId);
    ASSERT_NE(updatedCam, nullptr);
    EXPECT_EQ(updatedCam->target, targetId);
    EXPECT_TRUE(updatedCam->isActive);
}

TEST(CameraSystemTest, MoveToSetsPositionAndDisablesTracking) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<CameraComponent> cameras;
    CameraSystem system(transforms, cameras);

    EntityID camId = 1;
    cameras.Add(camId, CreateActiveCamera());
    system.SetActiveCamera(camId);

    SDL_Point target = {100, 200};
    system.MoveTo(target);

    auto* cam = cameras.Get(camId);
    ASSERT_NE(cam, nullptr);
    EXPECT_EQ(cam->position.x, target.x);
    EXPECT_EQ(cam->position.y, target.y);
    EXPECT_TRUE(cam->manualControl);
    EXPECT_EQ(cam->mode, CameraMode::Static);
    EXPECT_EQ(cam->target, INVALID_ENTITY);
}

TEST(CameraSystemTest, SetZoomClampsAndSetsModeCorrectly) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<CameraComponent> cameras;
    CameraSystem system(transforms, cameras);

    EntityID camId = 1;
    CameraComponent cam = CreateActiveCamera();
    cam.target = INVALID_ENTITY;
    cameras.Add(camId, cam);
    system.SetActiveCamera(camId);

    system.SetZoom(camId, 0.0f); // Should clamp to 0.1
    auto* updated = cameras.Get(camId);
    ASSERT_NE(updated, nullptr);
    EXPECT_FLOAT_EQ(updated->zoom, 0.1f);
    EXPECT_TRUE(updated->manualControl);
    EXPECT_EQ(updated->mode, CameraMode::Static);
}

TEST(CameraSystemTest, SetShakeAppliesIntensityAndDuration) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<CameraComponent> cameras;
    CameraSystem system(transforms, cameras);

    EntityID camId = 1;
    cameras.Add(camId, CreateActiveCamera());
    system.SetActiveCamera(camId);

    system.SetShake(20, -5.0f); // Should clamp both
    auto* cam = cameras.Get(camId);
    ASSERT_NE(cam, nullptr);
    EXPECT_EQ(cam->shakeIntensity, 15);
    EXPECT_FLOAT_EQ(cam->shakeDuration, 0.0f);
    EXPECT_EQ(cam->mode, CameraMode::Shake);
}

TEST(CameraSystemTest, UpdateCameraPositionFollowsTarget) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<CameraComponent> cameras;
    CameraSystem system(transforms, cameras);

    EntityID camId = 1;
    CameraComponent cam = CreateActiveCamera();
    cam.mode = CameraMode::Follow;
    cam.manualControl = false;
    cam.target = 2;
    cameras.Add(camId, cam);
    system.SetActiveCamera(camId);

    TransformComponent targetTransform{1000, 500};
    transforms.Add(2, targetTransform);

    system.Update(0.016f); // Simulate one frame

    auto* updated = cameras.Get(camId);
    ASSERT_NE(updated, nullptr);
    EXPECT_NE(updated->position.x, 0); // Should have moved
    EXPECT_NE(updated->position.y, 0);
}

TEST(CameraSystemTest, ClampToBoundsLimitsCameraPosition) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<CameraComponent> cameras;
    CameraSystem system(transforms, cameras);

    EntityID camId = 1;
    CameraComponent cam = CreateActiveCamera();
    cam.useBounds = true;
    cam.bounds = {0, 0, 500, 500}; // Smaller than viewport
    cam.position = {1000, 1000};   // Outside bounds
    cameras.Add(camId, cam);
    system.SetActiveCamera(camId);

    system.Update(0.016f);

    auto* updated = cameras.Get(camId);
    ASSERT_NE(updated, nullptr);
    EXPECT_LE(updated->position.x, cam.bounds.x + cam.bounds.w - cam.viewportSize.x);
    EXPECT_LE(updated->position.y, cam.bounds.y + cam.bounds.h - cam.viewportSize.y);
}

#include <gtest/gtest.h>
#include "core/World.h"
#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"
#include "components/BoundryComponent.h"
#include "components/CameraComponent.h"
#include "core/EntityManager.h"
#include "core/SystemManager.h"
#include "systems/CameraSystem.h"

class WorldTest : public ::testing::Test {
protected:
    World world;
};

// COMPONENT STORAGE TESTS
TEST_F(WorldTest, AddAndGetComponentStorage) {
    auto storage = std::make_unique<ComponentStorage<TransformComponent>>();
    world.AddComponentStorage<TransformComponent>(std::move(storage));

    EXPECT_TRUE(world.HasComponentStorage<TransformComponent>());

    auto& retrieved = world.GetComponentStorage<TransformComponent>();
    TransformComponent tc{1, 2};
    retrieved.Add(1, tc);

    auto* result = retrieved.Get(1);
    ASSERT_NE(result, nullptr);
    EXPECT_FLOAT_EQ(result->x, 1);
    EXPECT_FLOAT_EQ(result->y, 2);
}

TEST_F(WorldTest, RemoveComponentStorage) {
    world.AddComponentStorage<BoundryComponent>(std::make_unique<ComponentStorage<BoundryComponent>>());
    EXPECT_TRUE(world.HasComponentStorage<BoundryComponent>());

    world.RemoveComponentStorage<BoundryComponent>();
    EXPECT_FALSE(world.HasComponentStorage<BoundryComponent>());
    EXPECT_THROW(world.GetComponentStorage<BoundryComponent>(), std::runtime_error);
}

// SYSTEM TESTS
TEST_F(WorldTest, AddAndGetCameraSystem) {
    auto transformStorage = std::make_unique<ComponentStorage<TransformComponent>>();
    auto cameraStorage = std::make_unique<ComponentStorage<CameraComponent>>();

    world.AddComponentStorage<TransformComponent>(std::move(transformStorage));
    world.AddComponentStorage<CameraComponent>(std::move(cameraStorage));

    auto& transforms = world.GetComponentStorage<TransformComponent>();
    auto& cameras = world.GetComponentStorage<CameraComponent>();

    auto cameraSystem = std::make_unique<CameraSystem>(transforms, cameras);
    world.AddSystem<CameraSystem>(std::move(cameraSystem));

    auto& retrieved = world.GetSystem<CameraSystem>();
    EXPECT_NO_THROW(retrieved.Update(0.0f));
}

TEST_F(WorldTest, RemoveCameraSystem) {
    auto transformStorage = std::make_unique<ComponentStorage<TransformComponent>>();
    auto cameraStorage = std::make_unique<ComponentStorage<CameraComponent>>();

    world.AddComponentStorage<TransformComponent>(std::move(transformStorage));
    world.AddComponentStorage<CameraComponent>(std::move(cameraStorage));

    auto& transforms = world.GetComponentStorage<TransformComponent>();
    auto& cameras = world.GetComponentStorage<CameraComponent>();

    auto cameraSystem = std::make_unique<CameraSystem>(transforms, cameras);
    world.AddSystem<CameraSystem>(std::move(cameraSystem));

    world.RemoveSystem<CameraSystem>();
    EXPECT_THROW(world.GetSystem<CameraSystem>(), std::runtime_error);
}

// MANAGER TESTS
TEST_F(WorldTest, SetAndGetEntityManager) {
    EntityManager manager;
    world.SetEntityManager(manager);
    EXPECT_NO_THROW(world.GetEntityManager());
}

TEST_F(WorldTest, RemoveEntityManager) {
    EntityManager manager;
    world.SetEntityManager(manager);
    world.RemoveEntityManager();
    EXPECT_THROW(world.GetEntityManager(), std::runtime_error);
}

TEST_F(WorldTest, SetAndGetSystemManager) {
    SystemManager manager;
    world.SetSystemManager(manager);
    EXPECT_NO_THROW(world.GetSystemManager());
}

TEST_F(WorldTest, RemoveSystemManager) {
    SystemManager manager;
    world.SetSystemManager(manager);
    world.RemoveSystemManager();
    EXPECT_THROW(world.GetSystemManager(), std::runtime_error);
}

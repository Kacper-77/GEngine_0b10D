#include "systems/CollisionSystem.h"
#include "components/ColliderComponent.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include <gtest/gtest.h>

class CollisionSystemTest : public ::testing::Test {
protected:
    EntityManager entityManager;
    ComponentStorage<ColliderComponent> colliders;
    CollisionSystem system{entityManager, colliders};
};

TEST_F(CollisionSystemTest, DetectsCollisionBetweenTwoEntities) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();

    colliders.Add(a, ColliderComponent{0.0f, 0.0f, 10.0f, 10.0f});
    colliders.Add(b, ColliderComponent{5.0f, 5.0f, 10.0f, 10.0f}); //collision

    testing::internal::CaptureStdout();
    system.Update(0.0f);
    std::string output = testing::internal::GetCapturedStdout();

    ASSERT_NE(output.find("Collision detected between Entity"), std::string::npos);
}

TEST_F(CollisionSystemTest, DoesNotDetectCollisionWhenEntitiesAreFarApart) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();

    colliders.Add(a, ColliderComponent{0.0f, 0.0f, 10.0f, 10.0f});
    colliders.Add(b, ColliderComponent{100.0f, 100.0f, 10.0f, 10.0f}); // no collision

    testing::internal::CaptureStdout();
    system.Update(0.0f);
    std::string output = testing::internal::GetCapturedStdout();

    ASSERT_EQ(output.find("Collision detected between Entity"), std::string::npos);
}

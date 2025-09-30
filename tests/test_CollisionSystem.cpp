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

    system.SetCollisionCallback([](EntityID a, EntityID b) {
        std::cout << "Boom! Entity " << a << " hit Entity " << b << "\n";
    });

    testing::internal::CaptureStdout();
    system.Update(0.0f);
    std::string output = testing::internal::GetCapturedStdout();

    ASSERT_NE(output.find("Boom!"), std::string::npos);
}

TEST_F(CollisionSystemTest, DoesNotDetectCollisionWhenEntitiesAreFarApart) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();

    colliders.Add(a, ColliderComponent{0.0f, 0.0f, 10.0f, 10.0f});
    colliders.Add(b, ColliderComponent{100.0f, 100.0f, 10.0f, 10.0f}); // no collision

    system.SetCollisionCallback([](EntityID a, EntityID b) {
        std::cout << "Nothing\n";
    });

    testing::internal::CaptureStdout();
    system.Update(0.0f);
    std::string output = testing::internal::GetCapturedStdout();

    ASSERT_EQ(output.find("Nothing"), std::string::npos);
}

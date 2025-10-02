#include "components/ColliderComponent.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include "systems/CollisionSystem.h"
#include "event/core/EventBus.h"
#include "event/custom_events/CollisionEvent.h"

#include <gtest/gtest.h>
#include <vector>

class CollisionSystemTest : public ::testing::Test {
protected:
    EntityManager entityManager;
    ComponentStorage<ColliderComponent> colliders;
    EventBus eventBus;

    std::vector<CollisionEvent> receivedCollisions;

    CollisionSystem system{entityManager, colliders, eventBus};

    void SetUp() override {
        eventBus.Subscribe<CollisionEvent>([&](const CollisionEvent& e) {
            receivedCollisions.push_back(e);
        });
    }
};

TEST_F(CollisionSystemTest, EmitsCollisionEventWhenEntitiesOverlap) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();

    colliders.Add(a, ColliderComponent{0, 0, 10, 10});
    colliders.Add(b, ColliderComponent{5, 5, 10, 10}); // collision

    system.Update(0.0f);
    eventBus.Dispatch();

    ASSERT_EQ(receivedCollisions.size(), 1);
    EXPECT_EQ(receivedCollisions[0].entityA, a);
    EXPECT_EQ(receivedCollisions[0].entityB, b);
}

TEST_F(CollisionSystemTest, DoesNotEmitCollisionEventWhenEntitiesDoNotOverlap) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();

    colliders.Add(a, ColliderComponent{0, 0, 10, 10});
    colliders.Add(b, ColliderComponent{100, 100, 10, 10}); // no collision

    system.Update(0.0f);
    eventBus.Dispatch();

    ASSERT_TRUE(receivedCollisions.empty());
}

TEST_F(CollisionSystemTest, IgnoresEntitiesWithoutColliderComponent) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();

    colliders.Add(a, ColliderComponent{0, 0, 10, 10});
    // b has no collider

    system.Update(0.0f);
    eventBus.Dispatch();

    ASSERT_TRUE(receivedCollisions.empty());
}

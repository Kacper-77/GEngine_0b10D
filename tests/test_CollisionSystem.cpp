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
        // Subscribe to CollisionEvent and store any received events for verification
        eventBus.Subscribe<CollisionEvent>([&](const CollisionEvent& e) {
            receivedCollisions.push_back(e);
        });
    }

    // Utility for checking if a specific pair of entities exists in received collisions
    bool HasCollision(EntityID a, EntityID b) const {
        for (const auto& e : receivedCollisions) {
            if ((e.entityA == a && e.entityB == b) ||
                (e.entityA == b && e.entityB == a)) {
                return true;
            }
        }
        return false;
    }
};

// Test case: two overlapping entities should emit one collision event
TEST_F(CollisionSystemTest, EmitsCollisionEventWhenEntitiesOverlap) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();

    colliders.Add(a, ColliderComponent{0, 0, 10, 10});
    colliders.Add(b, ColliderComponent{5, 5, 10, 10}); // overlap with a

    system.Update(0.0f);
    eventBus.Dispatch();

    ASSERT_EQ(receivedCollisions.size(), 1);
    EXPECT_TRUE(HasCollision(a, b));
}

// Test case: two non-overlapping entities should not emit any collision
TEST_F(CollisionSystemTest, DoesNotEmitCollisionEventWhenEntitiesDoNotOverlap) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();

    colliders.Add(a, ColliderComponent{0, 0, 10, 10});
    colliders.Add(b, ColliderComponent{100, 100, 10, 10}); // far apart

    system.Update(0.0f);
    eventBus.Dispatch();

    ASSERT_TRUE(receivedCollisions.empty());
}

// Test case: system should ignore entities without collider components
TEST_F(CollisionSystemTest, IgnoresEntitiesWithoutColliderComponent) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();

    colliders.Add(a, ColliderComponent{0, 0, 10, 10});
    // b has no collider

    system.Update(0.0f);
    eventBus.Dispatch();

    ASSERT_TRUE(receivedCollisions.empty());
}

// Test case: system should emit multiple collision events if multiple entity pairs collide
TEST_F(CollisionSystemTest, EmitsMultipleCollisionEventsForMultiplePairs) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();
    EntityID c = entityManager.CreateEntity();

    // All three overlap
    colliders.Add(a, ColliderComponent{0, 0, 10, 10});
    colliders.Add(b, ColliderComponent{5, 5, 10, 10});
    colliders.Add(c, ColliderComponent{8, 8, 10, 10});

    system.Update(0.0f);
    eventBus.Dispatch();

    ASSERT_EQ(receivedCollisions.size(), 3);
    EXPECT_TRUE(HasCollision(a, b));
    EXPECT_TRUE(HasCollision(a, c));
    EXPECT_TRUE(HasCollision(b, c));
}

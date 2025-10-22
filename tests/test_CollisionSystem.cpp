#include "components/TransformComponent.h"
#include "components/ColliderComponent.h"
#include "components/PhysicsComponent.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include "systems/CollisionSystem.h"
#include "event/core/EventBus.h"
#include "event/custom_events/CollisionEvent.h"
#include "systems/EntityCreationSystem.h"

#include <gtest/gtest.h>
#include <vector>

class CollisionSystemTest : public ::testing::Test {
protected:
    EntityManager entityManager;
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<ColliderComponent> colliders;
    ComponentStorage<PhysicsComponent> physics;
    EventBus eventBus;
    EntityCreationSystem creationSystem{&entityManager};

    std::vector<CollisionEvent> receivedCollisions;

    CollisionSystem system{entityManager, transforms, colliders, physics, eventBus};

    void SetUp() override {
        creationSystem.RegisterStorage(&transforms);
        creationSystem.RegisterStorage(&colliders);

        eventBus.Subscribe<CollisionEvent>([&](const CollisionEvent& e) {
            receivedCollisions.push_back(e);
        });
    }

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

TEST_F(CollisionSystemTest, EmitsCollisionEventWhenEntitiesOverlap) {
    EntityID a = creationSystem.CreateEntityWith(
        TransformComponent{0, 0, 0, 0},
        ColliderComponent{10, 10}
    );
    EntityID b = creationSystem.CreateEntityWith(
        TransformComponent{5, 5, 0, 0},
        ColliderComponent{10, 10}
    );

    system.Update(0.0f);
    eventBus.Dispatch();

    ASSERT_EQ(receivedCollisions.size(), 1);
    EXPECT_TRUE(HasCollision(a, b));
}

TEST_F(CollisionSystemTest, DoesNotEmitCollisionEventWhenEntitiesDoNotOverlap) {
    EntityID a = creationSystem.CreateEntityWith(
        TransformComponent{0, 0, 0, 0},
        ColliderComponent{10, 10}
    );
    EntityID b = creationSystem.CreateEntityWith(
        TransformComponent{100, 100, 0, 0},
        ColliderComponent{10, 10}
    );

    system.Update(0.0f);
    eventBus.Dispatch();

    ASSERT_TRUE(receivedCollisions.empty());
}

TEST_F(CollisionSystemTest, IgnoresEntitiesWithoutColliderComponent) {
    EntityID a = creationSystem.CreateEntityWith(
        TransformComponent{0, 0, 0, 0},
        ColliderComponent{10, 10}
    );
    EntityID b = creationSystem.CreateEntityWith(
        TransformComponent{5, 5, 0, 0}
    );

    system.Update(0.0f);
    eventBus.Dispatch();

    ASSERT_TRUE(receivedCollisions.empty());
}

TEST_F(CollisionSystemTest, EmitsMultipleCollisionEventsForMultiplePairs) {
    EntityID a = creationSystem.CreateEntityWith(
        TransformComponent{0, 0, 0, 0},
        ColliderComponent{10, 10}
    );
    EntityID b = creationSystem.CreateEntityWith(
        TransformComponent{5, 5, 0, 0},
        ColliderComponent{10, 10}
    );
    EntityID c = creationSystem.CreateEntityWith(
        TransformComponent{8, 8, 0, 0},
        ColliderComponent{10, 10}
    );

    system.Update(0.0f);
    eventBus.Dispatch();

    ASSERT_EQ(receivedCollisions.size(), 3);
    EXPECT_TRUE(HasCollision(a, b));
    EXPECT_TRUE(HasCollision(a, c));
    EXPECT_TRUE(HasCollision(b, c));
}

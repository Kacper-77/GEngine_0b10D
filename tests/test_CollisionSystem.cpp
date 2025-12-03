#include "components/TransformComponent.h"
#include "components/ColliderComponent.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include "systems/CollisionSystem.h"
#include "systems/EntityCreationSystem.h"

#include <gtest/gtest.h>
#include <vector>

class CollisionSystemTest : public ::testing::Test {
protected:
    EntityManager entityManager;
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<ColliderComponent> colliders;
    EntityCreationSystem creationSystem{&entityManager};

    CollisionSystem system{entityManager, transforms, colliders};

    void SetUp() override {
        creationSystem.RegisterStorage(&transforms);
        creationSystem.RegisterStorage(&colliders);
    }

    bool HasCollision(EntityID a, EntityID b) const {
        for (const auto& pair : system.GetCollisions()) {
            if ((pair.first == a && pair.second == b) ||
                (pair.first == b && pair.second == a)) {
                return true;
            }
        }
        return false;
    }
};

TEST_F(CollisionSystemTest, DetectsCollisionWhenEntitiesOverlap) {
    EntityID a = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{0.0f, 0.0f}, 0.0f, VectorFloat{0.0f, 0.0f} },
        ColliderComponent{10, 10, CollisionLayer::Player, CollisionLayer::All}
    );
    EntityID b = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{5.0f, 5.0f}, 0.0f, VectorFloat{0.0f, 0.0f} },
        ColliderComponent{10, 10, CollisionLayer::Enemy, CollisionLayer::All}
    );

    system.Update(0.0f);

    ASSERT_EQ(system.GetCollisions().size(), 1);
    EXPECT_TRUE(HasCollision(a, b));
}

TEST_F(CollisionSystemTest, NoCollisionWhenEntitiesDoNotOverlap) {
    EntityID a = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{0.0f, 0.0f}, 0.0f, VectorFloat{0.0f, 0.0f} },
        ColliderComponent{10, 10, CollisionLayer::Player, CollisionLayer::All}
    );
    EntityID b = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{100.0f, 100.0f}, 0.0f, VectorFloat{0.0f, 0.0f} },
        ColliderComponent{10, 10, CollisionLayer::Enemy, CollisionLayer::All}
    );

    system.Update(0.0f);

    ASSERT_TRUE(system.GetCollisions().empty());
}

TEST_F(CollisionSystemTest, IgnoresEntitiesWithoutColliderComponent) {
    EntityID a = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{0.0f, 0.0f}, 0.0f, VectorFloat{0.0f, 0.0f} },
        ColliderComponent{10, 10, CollisionLayer::Player, CollisionLayer::All}
    );
    EntityID b = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{5.0f, 5.0f}, 0.0f, VectorFloat{0.0f, 0.0f} }
    );

    system.Update(0.0f);

    ASSERT_TRUE(system.GetCollisions().empty());
}

TEST_F(CollisionSystemTest, DetectsMultipleCollisionsForMultiplePairs) {
    EntityID a = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{0.0f, 0.0f}, 0.0f, VectorFloat{0.0f, 0.0f} },
        ColliderComponent{10, 10, CollisionLayer::Player, CollisionLayer::All}
    );
    EntityID b = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{5.0f, 5.0f}, 0.0f, VectorFloat{0.0f, 0.0f} },
        ColliderComponent{10, 10, CollisionLayer::Enemy, CollisionLayer::All}
    );
    EntityID c = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{8.0f, 8.0f}, 0.0f, VectorFloat{0.0f, 0.0f} },
        ColliderComponent{10, 10, CollisionLayer::Projectile, CollisionLayer::All}
    );

    system.Update(0.0f);

    ASSERT_EQ(system.GetCollisions().size(), 3);
    EXPECT_TRUE(HasCollision(a, b));
    EXPECT_TRUE(HasCollision(a, c));
    EXPECT_TRUE(HasCollision(b, c));
}
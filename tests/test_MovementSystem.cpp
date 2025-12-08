#include <gtest/gtest.h>
#include "systems/MovementSystem.h"
#include "core/EntityManager.h"
#include "systems/EntityCreationSystem.h"
#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "components/AccelerationComponent.h"
#include "components/PhysicsComponent.h" 

class MovementSystemTest : public ::testing::Test {
protected:
    EntityManager entityManager;
    EntityCreationSystem creationSystem{&entityManager};

    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<VelocityComponent> velocities;
    ComponentStorage<AccelerationComponent> accelerations;
    ComponentStorage<PhysicsComponent> physics;

    void SetUp() override {
        creationSystem.RegisterStorage(&transforms);
        creationSystem.RegisterStorage(&velocities);
        creationSystem.RegisterStorage(&accelerations);
        creationSystem.RegisterStorage(&physics);

        entityManager.RegisterComponentStorage(&transforms);
        entityManager.RegisterComponentStorage(&velocities);
        entityManager.RegisterComponentStorage(&accelerations);
        entityManager.RegisterComponentStorage(&physics);
    }
};

TEST_F(MovementSystemTest, MovesEntityWithoutPhysics) {
    EntityID entity = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{0.0f, 0.0f}, 0.0f, VectorFloat{64.0f, 64.0f} },
        VelocityComponent{0.0f, 0.0f},
        AccelerationComponent{10.0f, 5.0f}
    );

    MovementSystem system(transforms, velocities, accelerations, physics);

    float dt = 1.0f / 60.0f;
    system.Update(dt);

    auto* transform = transforms.Get(entity);
    auto* velocity = velocities.Get(entity);

    ASSERT_NE(transform, nullptr);
    ASSERT_NE(velocity, nullptr);

    // Velocity updated
    EXPECT_FLOAT_EQ(velocity->dx, 10.0f * dt);
    EXPECT_FLOAT_EQ(velocity->dy, 5.0f * dt);

    // Transform updated
    EXPECT_FLOAT_EQ(transform->position.x, (10.0f * dt) * dt);
    EXPECT_FLOAT_EQ(transform->position.y, (5.0f * dt) * dt);
}

TEST_F(MovementSystemTest, DoesNotAffectEntityWithPhysics) {
    EntityID entity = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{0.0f, 0.0f}, 0.0f, VectorFloat{64.0f, 64.0f} },
        VelocityComponent{2.0f, 3.0f},
        AccelerationComponent{10.0f, 5.0f},
        PhysicsComponent{} // Physics
    );

    MovementSystem system(transforms, velocities, accelerations, physics);

    float dt = 1.0f / 60.0f;
    system.Update(dt);

    auto* transform = transforms.Get(entity);
    auto* velocity = velocities.Get(entity);

    ASSERT_NE(transform, nullptr);
    ASSERT_NE(velocity, nullptr);

    // Nothing should happend
    EXPECT_FLOAT_EQ(velocity->dx, 2.0f);
    EXPECT_FLOAT_EQ(velocity->dy, 3.0f);

    EXPECT_FLOAT_EQ(transform->position.x, 0.0f);
    EXPECT_FLOAT_EQ(transform->position.y, 0.0f);
}

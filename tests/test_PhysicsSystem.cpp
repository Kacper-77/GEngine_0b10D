#include <gtest/gtest.h>
#include "systems/PhysicsSystem.h"
#include "core/EntityManager.h"
#include "systems/EntityCreationSystem.h"
#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"
#include "components/PhysicsComponent.h"
#include "components/AccelerationComponent.h"

class PhysicsSystemTest : public ::testing::Test {
protected:
    EntityManager entityManager;
    EntityCreationSystem creationSystem{&entityManager};

    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<PhysicsComponent> physics;
    ComponentStorage<AccelerationComponent> accelerations;

    void SetUp() override {
        creationSystem.RegisterStorage(&transforms);
        creationSystem.RegisterStorage(&physics);
        creationSystem.RegisterStorage(&accelerations);

        entityManager.RegisterComponentStorage(&transforms);
        entityManager.RegisterComponentStorage(&physics);
        entityManager.RegisterComponentStorage(&accelerations);
    }
};

TEST_F(PhysicsSystemTest, DoesPhysicsAffectTransform) {
    // Create entity with transform, acceleration and physics
    EntityID player = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{0.0f, 0.0f}, 0.0f, VectorFloat{64.0f, 64.0f} },
        AccelerationComponent{10.0f, 0.0f},
        PhysicsComponent{} // default physics
    );

    // Adjust physics to match expected behavior
    auto* phys = physics.Get(player);
    ASSERT_NE(phys, nullptr);

    phys->SetMass(1.0f);          // mass = 1 → acceleration = force
    phys->gravityScale = 1.0f;    // enable gravity
    phys->linearDamping = 0.0f;
    phys->velocity = {0, 0};      // start at rest
    phys->force = {0, 0};         // no external forces
    phys->impulse = {0, 0};       // no impulses

    PhysicsSystem system(transforms, accelerations, physics);
    system.Update(1.0f);

    auto* transform = transforms.Get(player);
    ASSERT_NE(transform, nullptr);

    EXPECT_FLOAT_EQ(phys->velocity.x, 10.0f);
    EXPECT_FLOAT_EQ(phys->velocity.y, 9.81f);

    EXPECT_FLOAT_EQ(transform->position.x, 10.0f);
    EXPECT_FLOAT_EQ(transform->position.y, 9.81f);
}


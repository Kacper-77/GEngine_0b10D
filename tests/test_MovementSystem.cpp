#include <gtest/gtest.h>
#include "systems/MovementSystem.h"
#include "core/EntityManager.h"
#include "systems/EntityCreationSystem.h"
#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "components/AccelerationComponent.h"

class MovementSystemTest : public ::testing::Test {
protected:
    EntityManager entityManager;
    EntityCreationSystem creationSystem{&entityManager};

    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<VelocityComponent> velocities;
    ComponentStorage<AccelerationComponent> accelerations;

    void SetUp() override {
        creationSystem.RegisterStorage(&transforms);
        creationSystem.RegisterStorage(&velocities);
        creationSystem.RegisterStorage(&accelerations);

        entityManager.RegisterComponentStorage(&transforms);
        entityManager.RegisterComponentStorage(&velocities);
        entityManager.RegisterComponentStorage(&accelerations);
    }
};

TEST_F(MovementSystemTest, AppliesAccelerationToVelocityAndMovesEntity) {
    EntityID player = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{0.0f, 0.0f}, 0.0f, VectorFloat{64.0f, 64.0f} },
        VelocityComponent{0.0f, 0.0f},
        AccelerationComponent{10.0f, 5.0f}
    );

    MovementSystem system(transforms, velocities, accelerations);
    system.Update(1.0f); // deltaTime = 1.0f

    auto* transform = transforms.Get(player);
    auto* velocity = velocities.Get(player);

    ASSERT_NE(transform, nullptr);
    ASSERT_NE(velocity, nullptr);

    EXPECT_FLOAT_EQ(velocity->dx, 10.0f);
    EXPECT_FLOAT_EQ(velocity->dy, 5.0f);
    EXPECT_EQ(transform->position.x, 10);
    EXPECT_EQ(transform->position.y, 5);
}

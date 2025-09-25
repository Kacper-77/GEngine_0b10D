#include <gtest/gtest.h>
#include "systems/MovementSystem.h"

TEST(MovementSystemTest, AppliesAccelerationToVelocityAndMovesEntity) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<VelocityComponent> velocities;
    ComponentStorage<AccelerationComponent> accelerations;

    Entity player(2);
    transforms.Add(player.GetID(), {0, 0, 64, 64});
    velocities.Add(player.GetID(), {0.0f, 0.0f});
    accelerations.Add(player.GetID(), {10.0f, 5.0f});

    MovementSystem system(transforms, velocities, accelerations);
    system.Update(1.0f);

    auto* transform = transforms.Get(player.GetID());
    auto* velocity = velocities.Get(player.GetID());

    ASSERT_NE(transform, nullptr);
    ASSERT_NE(velocity, nullptr);

    EXPECT_FLOAT_EQ(velocity->dx, 10.0f);
    EXPECT_FLOAT_EQ(velocity->dy, 5.0f);
    EXPECT_EQ(transform->x, 10);
    EXPECT_EQ(transform->y, 5);
}

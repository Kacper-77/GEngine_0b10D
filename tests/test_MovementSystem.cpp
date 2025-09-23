#include <gtest/gtest.h>
#include "systems/MovementSystem.h"
#include "ComponentStorage.h"

TEST(MovementSystemTest, MovesEntityCorrectly) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<VelocityComponent> velocities;

    Entity player(1);
    transforms.Add(player.GetID(), {0, 0, 64, 64});
    velocities.Add(player.GetID(), {100.0f, 50.0f});

    MovementSystem system(transforms, velocities);
    system.Update(1.0f);

    auto* transform = transforms.Get(player.GetID());
    ASSERT_NE(transform, nullptr);
    EXPECT_EQ(transform->x, 100);
    EXPECT_EQ(transform->y, 50);
}

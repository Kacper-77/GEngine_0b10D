#include <gtest/gtest.h>
#include "core/systems/PhysicsSystem.h"

TEST(PhysicsSystemTest, DoesPhysicsAffectTransform) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<PhysicsComponent> physics;
    ComponentStorage<AccelerationComponent> accelerations;

    Entity player(7);

    transforms.Add(player.GetID(), {0, 0, 64, 64});
    accelerations.Add(player.GetID(), {10.0f, 0.0f});
    physics.Add(player.GetID(), {0.0f, 0.0f, 0.0f, 9.8f});

    PhysicsSystem system(transforms, accelerations, physics);
    system.Update(1.0f);

    auto* transform = transforms.Get(player.GetID());
    auto* phys = physics.Get(player.GetID());

    ASSERT_NE(transform, nullptr);
    ASSERT_NE(phys, nullptr);

    ASSERT_FLOAT_EQ(phys->posX, 10.0f);
    ASSERT_FLOAT_EQ(phys->posY, 9.8f);
    
    ASSERT_EQ(transform->x, 10);
    ASSERT_EQ(transform->y, 9);
}
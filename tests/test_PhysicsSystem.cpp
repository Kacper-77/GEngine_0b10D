#include <gtest/gtest.h>
#include "systems/PhysicsSystem.h"
#include "core/EntityManager.h"

TEST(PhysicsSystemTest, DoesPhysicsAffectTransform) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<PhysicsComponent> physics;
    ComponentStorage<AccelerationComponent> accelerations;

    EntityManager entityManager;
    EntityID player = entityManager.CreateEntity();

    transforms.Add(player, {0, 0, 64, 64});
    accelerations.Add(player, {10.0f, 0.0f});
    physics.Add(player, {0.0f, 0.0f, 0.0f, 9.8f});

    PhysicsSystem system(transforms, accelerations, physics);
    system.Update(1.0f);

    auto* transform = transforms.Get(player);
    auto* phys = physics.Get(player);

    ASSERT_NE(transform, nullptr);
    ASSERT_NE(phys, nullptr);

    ASSERT_FLOAT_EQ(phys->posX, 10.0f);
    ASSERT_FLOAT_EQ(phys->posY, 9.8f);
    
    ASSERT_EQ(transform->x, 10);
    ASSERT_EQ(transform->y, 9);
}
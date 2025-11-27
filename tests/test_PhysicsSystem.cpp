#include <gtest/gtest.h>
#include "systems/PhysicsSystem.h"
#include "core/EntityManager.h"
#include "systems/EntityCreationSystem.h"
#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"
#include "components/PhysicsComponent.h"
#include "components/AccelerationComponent.h"
#include "components/ForceComponent.h"

class PhysicsSystemTest : public ::testing::Test {
protected:
    EntityManager entityManager;
    EntityCreationSystem creationSystem{&entityManager};

    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<PhysicsComponent> physics;
    ComponentStorage<AccelerationComponent> accelerations;
    ComponentStorage<ForceComponent> forces;

    void SetUp() override {
        creationSystem.RegisterStorage(&transforms);
        creationSystem.RegisterStorage(&physics);
        creationSystem.RegisterStorage(&accelerations);
        creationSystem.RegisterStorage(&forces);

        entityManager.RegisterComponentStorage(&transforms);
        entityManager.RegisterComponentStorage(&physics);
        entityManager.RegisterComponentStorage(&accelerations);
        entityManager.RegisterComponentStorage(&forces);
    }
};

TEST_F(PhysicsSystemTest, DoesPhysicsAffectTransform) {
    EntityID player = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{0.0f, 0.0f}, 0.0f, VectorFloat{64.0f, 64.0f} },
        AccelerationComponent{10.0f, 0.0f},
        PhysicsComponent{0.0f, 0.0f, 0.0f, 9.8f}
    );

    PhysicsSystem system(transforms, accelerations, forces, physics);
    system.Update(1.0f); // deltaTime = 1.0f

    auto* transform = transforms.Get(player);
    auto* phys = physics.Get(player);

    ASSERT_NE(transform, nullptr);
    ASSERT_NE(phys, nullptr);

    ASSERT_FLOAT_EQ(phys->posX, 10.0f);
    ASSERT_FLOAT_EQ(phys->posY, 9.8f);

    ASSERT_EQ(transform->position.x, 10);
    ASSERT_EQ(transform->position.y, 9);
}

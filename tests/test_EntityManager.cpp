#include <gtest/gtest.h>
#include "core/EntityManager.h"

TEST(EntityManagerTest, CreateAndDestroyEntity) {
    EntityManager manager;
    EntityID id = manager.CreateEntity();
    ASSERT_TRUE(manager.IsAlive(id));
    ASSERT_EQ(id, 0);
    manager.DestroyEntity(id);
    ASSERT_FALSE(manager.IsAlive(id));
}
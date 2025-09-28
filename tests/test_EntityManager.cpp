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

TEST(EntityManagerTest, TaggingAndGrouping) {
    EntityManager manager;
    EntityID e1 = manager.CreateEntity();
    EntityID e2 = manager.CreateEntity();
    manager.AddTag(e1, "Enemy");
    manager.AddTag(e2, "Enemy");

    auto group = manager.GetGroup("Enemy");
    ASSERT_EQ(group.size(), 2);
    ASSERT_TRUE(group.count(e1));
    ASSERT_TRUE(group.count(e2));

    manager.RemoveTag(e1);
    group = manager.GetGroup("Enemy");
    ASSERT_EQ(group.size(), 1);
    ASSERT_FALSE(group.count(e1));
}
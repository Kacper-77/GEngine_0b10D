#include <gtest/gtest.h>
#include "core/EntityManager.h"
#include "systems/EntityCreationSystem.h"
#include "components/TransformComponent.h"
#include "core/ComponentStorage.h"

class EntityManagerTest : public ::testing::Test {
protected:
    EntityManager manager;
    EntityCreationSystem creationSystem{&manager};
    ComponentStorage<TransformComponent> transforms;

    void SetUp() override {
        creationSystem.RegisterStorage(&transforms);
        manager.RegisterComponentStorage(&transforms);
    }
};

TEST_F(EntityManagerTest, CreateAndDestroyEntity) {
    EntityID id = creationSystem.CreateEntityWith();
    ASSERT_TRUE(manager.IsAlive(id));
    ASSERT_EQ(id, 1);
    manager.DestroyEntityFromList(id);
    ASSERT_FALSE(manager.IsAlive(id));
}

TEST_F(EntityManagerTest, TaggingAndGrouping) {
    EntityID e1 = creationSystem.CreateEntityWith(std::string{"Enemy"});
    EntityID e2 = creationSystem.CreateEntityWith(std::string{"Enemy"});

    auto group = manager.GetGroup("Enemy");
    ASSERT_EQ(group.size(), 2);
    ASSERT_TRUE(group.count(e1));
    ASSERT_TRUE(group.count(e2));

    manager.RemoveTag(e1);
    group = manager.GetGroup("Enemy");
    ASSERT_EQ(group.size(), 1);
    ASSERT_FALSE(group.count(e1));
}

TEST_F(EntityManagerTest, DestroyAlsoRemovesComponents) {
    EntityID e = creationSystem.CreateEntityWith(TransformComponent{});
    ASSERT_TRUE(transforms.Has(e));
    manager.DestroyEntityFromList(e);
    ASSERT_FALSE(transforms.Has(e));
}

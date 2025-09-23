#include <gtest/gtest.h>
#include "ComponentStorage.h"

TEST(ComponentStorageTest, AddAndGetComponent) {
    ComponentStorage<int> storage;
    storage.Add(1, 42);
    auto* value = storage.Get(1);
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, 42);
}

TEST(ComponentStorageTest, RemoveComponent) {
    ComponentStorage<int> storage;
    storage.Add(2, 99);
    storage.Remove(2);
    EXPECT_EQ(storage.Get(2), nullptr);
}

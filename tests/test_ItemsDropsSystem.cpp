#include <gtest/gtest.h>
#include "systems/ItemsDropsSystem.h"

class ItemSystemTest : public ::testing::Test {
protected:
    ItemSystem itemSystem;

    void SetUp() override {
        // Register few items
        Stat swordStats{"ATK", 106};

        ItemDefinition sword{1, "Sword", Rarity::COMMON, ItemType::EQUIPMENT, {swordStats}, "Basic sword"};
        ItemDefinition potion{2, "Health Potion", Rarity::RARE, ItemType::CONSUMABLE, {}, "Restores HP"};
        ItemDefinition gem{3, "Epic Gem", Rarity::EPIC, ItemType::MATERIAL, {}, "Shiny gem"};

        itemSystem.RegisterItem(sword);
        itemSystem.RegisterItem(potion);
        itemSystem.RegisterItem(gem);
    }
};

// Check registered items
TEST_F(ItemSystemTest, RegisterAndGetDefinition) {
    const ItemDefinition* def = itemSystem.GetDefinition(1);
    ASSERT_NE(def, nullptr);
    EXPECT_EQ(def->itemName, "Sword");
    EXPECT_EQ(def->rarity, Rarity::COMMON);
    EXPECT_EQ(std::get<int>(def->baseStats[0].value), 106);

    def = itemSystem.GetDefinition(2);
    ASSERT_NE(def, nullptr);
    EXPECT_EQ(def->itemName, "Health Potion");
    EXPECT_EQ(def->type, ItemType::CONSUMABLE);
}

// Checking instance of items
TEST_F(ItemSystemTest, CreateInstance) {
    ItemInstance inst = itemSystem.CreateInstance(1, 2);
    EXPECT_EQ(inst.itemID, 1);
    EXPECT_EQ(inst.amount, 2);
    EXPECT_FALSE(inst.isBound);
    EXPECT_FALSE(inst.modifiers.has_value());
}

// High chance drops
TEST_F(ItemSystemTest, GenerateDropGuaranteed) {
    std::vector<DropInfo> dropTable = {
        {1, Rarity::COMMON, 1, 1.0f}, // 100% 
        {2, Rarity::RARE, 2, 1.0f}    // 100%
    };

    auto drops = itemSystem.GenerateDrop(dropTable);
    ASSERT_EQ(drops.size(), 2);
    EXPECT_EQ(drops[0].itemID, 1);
    EXPECT_EQ(drops[0].amount, 1);
    EXPECT_EQ(drops[1].itemID, 2);
    EXPECT_EQ(drops[1].amount, 2);
}

// Low chance drop
TEST_F(ItemSystemTest, GenerateDropNone) {
    std::vector<DropInfo> dropTable = {
        {3, Rarity::EPIC, 1, 0.0f} // 0%
    };

    auto drops = itemSystem.GenerateDrop(dropTable);
    EXPECT_TRUE(drops.empty());
}

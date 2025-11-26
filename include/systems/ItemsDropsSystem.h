#pragma once

#include "utils/ItemsAndDrops.h"

class ItemSystem {
public:
    // Register definition
    void RegisterItem(const ItemDefinition& def);

    // Get definition
    const ItemDefinition* GetDefinition(ItemID id) const;

    // Create instance of item
    ItemInstance CreateInstance(ItemID id, uint8_t amount = 1) const;

    // Generate drop
    std::vector<ItemInstance> GenerateDrop(const std::vector<DropInfo>& dropTable) const;

private:
    std::unordered_map<ItemID, ItemDefinition> m_definitions;

    float RandomFloat(float min = 0.0f, float max = 1.0f) const;
};

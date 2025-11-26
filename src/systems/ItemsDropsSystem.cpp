#include "systems/ItemsDropsSystem.h"

// Register definition
void ItemSystem::RegisterItem(const ItemDefinition& def) {
    m_definitions[def.itemID] = def;
}

// Get definition
const ItemDefinition* ItemSystem::GetDefinition(ItemID id) const {
    auto it = m_definitions.find(id);
    return it != m_definitions.end() ? &it->second : nullptr;
}

// Create instance of item
ItemInstance ItemSystem::CreateInstance(ItemID id, uint8_t amount) const {
    return ItemInstance{id, amount};
}

// Generate drop
std::vector<ItemInstance> ItemSystem::GenerateDrop(const std::vector<DropInfo>& dropTable) const {
    std::vector<ItemInstance> result;

    for (const auto& drop : dropTable) {
        float roll = RandomFloat();
        if (roll <= drop.dropChance) {
            result.push_back(CreateInstance(drop.itemID, drop.quantity));
        }
    }

    return result;
}

// Random float
float ItemSystem::RandomFloat(float min, float max) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

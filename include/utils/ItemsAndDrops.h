#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>
#include <random>

// Enums
enum class Rarity {
    COMMON,
    RARE,
    EPIC,
    LEGENDARY,
    CUSTOM
};

enum class ItemType {
    CONSUMABLE,
    EQUIPMENT,
    MATERIAL,
    QUEST,
    MISC
};

using ItemID = size_t;

// Stats
struct Stat {
    std::string name;
    std::variant<std::string, float, int> value;
};

// Define item
struct ItemDefinition {
    ItemID itemID;
    std::string itemName;
    Rarity rarity;
    ItemType type;
    std::vector<Stat> baseStats;
    std::string description;
    bool stackable = true;
    uint8_t maxStack = 99;   
};

// Item instance
struct ItemInstance {
    ItemID itemID;
    uint8_t amount;
    std::optional<std::vector<Stat>> modifiers; // random bonus
    bool isBound = false; // example - "quest item"
};

// Drop info
struct DropInfo {
    ItemID itemID;
    Rarity rarity;
    uint8_t quantity;
    float dropChance; // 0.0 - 1.0
};

// ItemSystem
class ItemSystem {
public:
    // Register definition
    void RegisterItem(const ItemDefinition& def) {
        m_definitions[def.itemID] = def;
    }

    // Get definition
    const ItemDefinition* GetDefinition(ItemID id) const {
        auto it = m_definitions.find(id);
        return it != m_definitions.end() ? &it->second : nullptr;
    }

    // Create instance of item
    ItemInstance CreateInstance(ItemID id, uint8_t amount = 1) const {
        ItemInstance inst{id, amount};
        return inst;
    }

    // Generate drop
    std::vector<ItemInstance> GenerateDrop(const std::vector<DropInfo>& dropTable) const {
        std::vector<ItemInstance> result;
        for (const auto& drop : dropTable) {
            float roll = RandomFloat();
            if (roll <= drop.dropChance) {
                result.push_back(CreateInstance(drop.itemID, drop.quantity));
            }
        }
        return result;
    }

private:
    std::unordered_map<ItemID, ItemDefinition> m_definitions;

    inline float RandomFloat(float min = 0.0f, float max = 1.0f) const {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }
};

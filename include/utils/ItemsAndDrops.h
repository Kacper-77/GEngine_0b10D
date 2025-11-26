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

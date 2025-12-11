# Item System 🪓

The **ItemSystem** manages item definitions, item instances, and randomized loot generation.  
It provides a clean and extensible foundation for inventory systems, enemy drops, chests, crafting rewards, and any gameplay mechanic that requires item creation or probability‑based loot tables.

---

## Overview

The system maintains a registry of item definitions (`ItemDefinition`) and can create concrete item instances (`ItemInstance`) on demand.  
It also supports probability‑based drop generation using a simple but powerful loot table structure (`DropInfo`).

This design keeps item logic centralized, deterministic, and easy to extend.

---

## Responsibilities

- Register item definitions at startup.
- Retrieve item definitions by ID.
- Create item instances with a specified quantity.
- Generate randomized drops based on probability tables.
- Provide a reusable RNG utility for consistent loot behavior.

---

## Item Definitions

Items are registered using:
RegisterItem(const ItemDefinition& def)

Each definition is stored in an internal map:

- `itemID` – unique identifier
- Additional metadata (name, rarity, stats, etc.) defined elsewhere

Definitions act as templates for creating item instances.

---

## Item Instances

To create an actual item stack, the system uses:
CreateInstance(ItemID id, uint8_t amount)

This produces a lightweight `ItemInstance` containing:

- The item ID
- The stack size

Instances are returned by value and can be stored in inventories, dropped into the world, or passed to other systems.

---

## Drop Generation

The system supports probability‑based loot tables via:
GenerateDrop(const std::vector<DropInfo>& dropTable)

Each `DropInfo` entry contains:

- `itemID` – which item to drop
- `dropChance` – probability (0.0–1.0)
- `quantity` – amount to drop if successful

### How it works

For each entry:

1. Roll a random float in `[0, 1]`
2. If `roll <= dropChance`, the item is added to the result list
3. Return all successful drops

This allows for:

- Enemy loot tables
- Chest rewards
- Randomized crafting outcomes
- Resource node yields

---

## Random Number Generation

The system uses a static `std::mt19937` PRNG seeded with `std::random_device`:
```RandomFloat(min, max)```

This ensures:

- High‑quality randomness
- Consistent behavior across calls
- No repeated seeding overhead

---

## Why This Design Is Strong

- **Modular**: Item definitions are decoupled from item instances.
- **Extensible**: Easy to add rarity, modifiers, crafting rules, etc.
- **Deterministic**: RNG is centralized and consistent.
- **Lightweight**: No unnecessary allocations or complex structures.
- **Gameplay‑ready**: Supports all common loot mechanics.

This is the same architecture used in RPGs, roguelikes, survival games, and action titles.

---

## Summary

The **ItemSystem** provides a clean and efficient foundation for:

- Item registration  
- Item instantiation  
- Probability‑based loot generation  

It is simple, scalable, and integrates seamlessly with inventory, combat, and world systems.

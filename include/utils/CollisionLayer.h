#pragma once
#include <cstdint>

enum class CollisionLayer : uint8_t {
    None         = 0,          // 00000000
    Player       = 1,          // 00000001
    Enemy        = 1 << 1,     // 00000010
    Projectile   = 1 << 2,     // 00000100
    Wall         = 1 << 3,     // 00001000 — wall/obstacle
    Pickup       = 1 << 4,     // 00010000 — pickup item
    Trigger      = 1 << 5,     // 00100000 — trigger box
    Sensor       = 1 << 6,     // 01000000 — example, invlisible sensor
    Environment  = 1 << 7,     // 10000000 — water, lava, traps etc.
    All          = 0xFF        // 11111111 — all
};

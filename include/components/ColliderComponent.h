#pragma once

#include "utils/CollisionLayer.h"

struct ColliderComponent {
    int width, height;
    CollisionLayer layer;  // Who
    CollisionLayer mask;   // With who I can collide
};

inline bool CanCollide(const ColliderComponent& a, const ColliderComponent& b) {
        return static_cast<uint8_t>(a.mask) & static_cast<uint8_t>(b.layer) ||
               static_cast<uint8_t>(b.mask) & static_cast<uint8_t>(a.layer);
    }

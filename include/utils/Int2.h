#pragma once

#include <functional>

struct Int2 {
    int x, y;
    bool operator==(const Int2& other) const { return x == other.x && y == other.y; }
};

namespace std {
    template <>
    struct hash<Int2> {
        std::size_t operator()(const Int2& k) const {
            return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1);
        }
    };
}

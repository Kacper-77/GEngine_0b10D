#pragma once

#include <string>
#include <unordered_map>
#include "utils/EntityTypes.h"

struct Event {
    std::string name; // "Bounce", "Damage", "Knockback"...
    EntityID source;
    EntityID target;
    std::unordered_map<std::string, float> data;
};

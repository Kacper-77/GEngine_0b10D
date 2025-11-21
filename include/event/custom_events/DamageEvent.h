#pragma once

#include <string>
#include "event/core/Event.h"
#include "utils/EntityTypes.h"

struct DamageEvent : public Event {
    std::optional<EntityID> dest;
    EntityID src = INVALID_ENTITY;
    int amount = 0;
    std::string type = "";
    std::string effect = "";
    float effectDuration = 0.0f;
    bool enableCritical = false;
    float criticalChance = 0.0f;
    float criticalBouns = 0.0f;

    explicit DamageEvent(std::optional<EntityID> dest)
        : dest{dest} {}
};
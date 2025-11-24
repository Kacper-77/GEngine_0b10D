#pragma once

#include <string>
#include <optional>
#include "event/core/Event.h"
#include "utils/EntityTypes.h"

struct DamageEvent : public Event {
    std::optional<EntityID> dest;
    EntityID src = INVALID_ENTITY;
    int amount = 0;
    std::string type = "";
    std::string effect = "";
    float effectDuration = 0.0f;
    bool isCriticalEnabled = false;
    float criticalChance = 0.0f;
    float criticalBonus = 0.0f;

    explicit DamageEvent(std::optional<EntityID> dest)
        : dest{dest} {}
};
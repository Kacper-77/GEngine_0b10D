#pragma once

#include <string>
#include "event/core/Event.h"
#include "utils/EntityTypes.h"

struct DamageEvent : public Event {
    EntityID dest;
    EntityID src = INVALID_ENTITY;
    int amount = 0;
    std::string type = "";
    std::string effect = "";
    float effectDuration = 0.0f;
    float criticalChance = 0.0f;

    explicit DamageEvent(EntityID dest)
        : dest{dest} {}
};
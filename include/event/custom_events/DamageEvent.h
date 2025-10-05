#pragma once

#include <string>
#include "../core/Event.h"
#include "../../utils/EntityTypes.h"

struct DamageEvent : public Event {
    EntityID dest;
    EntityID src = INVALID_ENTITY;
    float amount = 0.0f;
    std::string type = "";
    std::string effect = "";
    bool critical = false;

    explicit DamageEvent(EntityID dest)
        : dest{dest} {}
};
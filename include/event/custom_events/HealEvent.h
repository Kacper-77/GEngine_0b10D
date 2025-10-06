#pragma once

#include <string>
#include "../core/Event.h"
#include "../../utils/EntityTypes.h"

struct HealEvent : public Event {
    EntityID target;
    EntityID src = INVALID_ENTITY;
    float amount = 0.0f;
    std::string effect = "";
    std::string type = "";  // magic, item, etc.
    bool overheal = false;

    HealEvent(EntityID target)
        : target{target} {}
};
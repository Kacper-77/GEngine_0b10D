#pragma once

#include "../core/Event.h"
#include "../../utils/EntityTypes.h"
#include <string>

struct CollisionEvent : public Event {
    EntityID entityA;
    EntityID entityB;
    std::string typeA;
    std::string typeB;

    explicit CollisionEvent(EntityID a, EntityID b, const std::string& typeA, const std::string& typeB)
        : entityA{a}, entityB{b}, typeA{typeA}, typeB{typeB} {}
};
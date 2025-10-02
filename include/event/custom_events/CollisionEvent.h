#pragma once

#include "../core/Event.h"
#include "../../utils/EntityTypes.h"

struct CollisionEvent : public Event {
    EntityID entityA;
    EntityID entityB;

    CollisionEvent(EntityID a, EntityID b)
        : entityA{a}, entityB{b} {}
};
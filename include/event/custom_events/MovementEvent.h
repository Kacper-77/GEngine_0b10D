#pragma once

#include "../core/Event.h"
#include "../../utils/EntityTypes.h"

struct MovementEvent : public Event {
    EntityID entity;
    float speedAmount = 0.0f;
    float acceleration = 0.0f;
    float velocity = 0.0f;

    explicit MovementEvent(EntityID entity)
        : entity{entity} {}
};
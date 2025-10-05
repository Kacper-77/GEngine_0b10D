#pragma once

#include "../core/Event.h"
#include "../../utils/EntityTypes.h"

struct PhysicsEvent : public Event {
    EntityID entity = INVALID_ENTITY;
    float velocityAmount = 0.0f;
    float friction = 0.0f;
    float gravity = 0.0f;
    bool global = false;

    PhysicsEvent() = default;
    explicit PhysicsEvent(EntityID entity)
        : entity{entity} {}
};
#pragma once

#include "../core/Event.h"
#include "../../utils/EntityTypes.h"

struct TeleportEvent : public Event {
    EntityID entity;
    int x, y;
    bool instant = false;

    explicit TeleportEvent(EntityID entity, int x, int y)
        : entity{entity}, x{x}, y{y} {}
};
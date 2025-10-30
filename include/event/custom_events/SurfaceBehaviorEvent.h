#pragma once

#include "../core/Event.h"
#include "utils/EntityTypes.h"
#include "utils/SurfaceTypes.h"

struct SurfaceBehaviorEvent : public Event {
    EntityID entity;
    EntityID surface;

    SurfaceType surfaceType;
    float customVelocityX;
    float customVelocityY;

    SurfaceBehaviorEvent(EntityID entity, EntityID surface, SurfaceType surfaceType)
        : entity{entity}, surface{surface}, surfaceType{surfaceType} {}
};
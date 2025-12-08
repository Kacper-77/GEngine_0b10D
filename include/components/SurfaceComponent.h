#pragma once

#include "utils/SurfaceTypes.h"

struct SurfaceComponent {
    SurfaceType surfaceType;
    float multiplier = 1.0f;         
    float frictionMultiplier = 1.0f;



    static const char* ToString(SurfaceType type) {
        switch (type) {
            case SurfaceType::ICE:    return "ICE";
            case SurfaceType::SAND:   return "SAND";
            case SurfaceType::GRASS:  return "GRASS";
            case SurfaceType::LIQUID: return "LIQUID";
            case SurfaceType::CUSTOM: return "CUSTOM";
            default:                  return "UNKNOWN";
        }
    }
};

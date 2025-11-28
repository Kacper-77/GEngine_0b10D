#pragma once

#include "utils/SurfaceTypes.h"

struct SurfaceComponent {
    float x;
    float y;
    float width;
    float height;
    SurfaceType surfaceType;

    // Helpers
    bool Contains(float px, float py) const {
        return px >= x && px <= x + width &&
               py >= y && py <= y + height;
    }

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

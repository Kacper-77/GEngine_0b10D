#pragma once

#include "SDL.h"
#include "utils/EntityTypes.h"
#include "utils/Vector.h"

enum CameraMode {
    Static,
    Follow,
    SmoothFollow,
    Scripted,
    RoomBased,
    Shake
};

struct CameraComponent {
    // Basic
    EntityID target = INVALID_ENTITY;
    SDL_Point position;
    SDL_Point offset;
    SDL_Point viewportSize;
    float zoom = 1.0f;
    float rotationDegrees = 0.0f;

    // mode and settings
    CameraMode mode;
    int smoothing;
    SDL_Rect bounds = { 0, 0, 0, 0 };
    bool useBounds = false;
    bool manualControl = false;
    bool isActive = true;

    //Effects
    int shakeIntensity = 0;
    float shakeDuration = 0.0f;
    Uint8 fadeAlpha = 0;
    SDL_Point scriptTarget = { 0, 0 };
};
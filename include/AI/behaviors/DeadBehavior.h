#pragma once

#include "AI/AIBehavior.h"
#include "AI/AIController.h"
#include "components/VelocityComponent.h"

class DeadBehavior : public AIBehavior {
public:
    void UpdateAI(AIController& component, float dletaTime) override {
        // Check state of NPC
        if (component.GetState() != AIState::Dead) return;

        // Get velocity of current target
        VelocityComponent* v = component.GetVelocityComponent();
        if (!v) return;

        // NPC doesn't move because it's dead
        v->dx = 0.0f;
        v->dy = 0.0f;

        // ANIMATIONS LATER! (respawn, disappear etc.)
    }
};

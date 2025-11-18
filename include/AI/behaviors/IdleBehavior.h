#pragma once

#include "AI/AIBehavior.h"
#include "AI/AIController.h"
#include "components/VelocityComponent.h"

class IdleBehavior : public AIBehavior {
public:
    void UpdateAI(AIController& component, float dletaTime) override {
        // Check state of NPC
        if (component.GetState() != AIState::Idle) return;

        // Get velocity of current target
        VelocityComponent* v = component.GetVelocityComponent();
        if (!v) return;

        // NPC doesn't move
        v->dx = 0.0f;
        v->dy = 0.0f;

        // ANIMATIONS LATER!
    }
};

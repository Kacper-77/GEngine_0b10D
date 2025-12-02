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
        if (auto* v = component.GetVelocityComponent()) {
        // NPC doesn't move because it's dead
        v->dx = 0.0f;
        v->dy = 0.0f;
        }

        if (auto* anim = component.GetAnimationComponent()) {
            if (anim->stateMachine) {
                anim->stateMachine->currentState = "Dead";
            }
        }
    }
};

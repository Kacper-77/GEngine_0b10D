#pragma once

#include "AI/AIBehavior.h"
#include "AI/AIController.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"

class ChaseBehavior : public AIBehavior {
public:
    void UpdateAI(AIController& component, float deltaTime) override {
        // Check state of NPC
        if (component.GetState() != AIState::Chase) return;

        // Get component data
        TransformComponent* self = component.GetTransformComponent();
        VelocityComponent* v = component.GetVelocityComponent();
        TransformComponent* target = component.GetTargetTransform();
        if (!self || !v || !target) return;

        // Position
        VectorFloat selfPos{self->position.x, self->position.y};
        VectorFloat targetPos{target->position.x, target->position.y};

        // Calculate direction to target
        VectorFloat toTarget = targetPos - selfPos;
        if (toTarget.Length() == 0) return;

        VectorFloat dir = toTarget.Normalized();

        // Speed depends on movement mode (walk/run)
        float speed = component.GetSpeed();
        v->dx = dir.x * speed;
        v->dy = dir.y * speed;

        // Trigger chase animation if available
        if (auto* anim = component.GetAnimationComponent()) {
            if (anim->stateMachine) {
                anim->stateMachine->currentState = component.IsRunning() ? "Run" : "Walk";
            }
        }
    }
};

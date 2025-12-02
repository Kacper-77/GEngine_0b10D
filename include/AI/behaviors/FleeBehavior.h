#pragma once

#include "AI/AIBehavior.h"
#include "AI/AIController.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"

class FleeBehavior : public AIBehavior {
public:
    void UpdateAI(AIController& component, float deltaTime) override {
        // Check state of NPC
        if (component.GetState() != AIState::Flee) return;

        // Get component data
        TransformComponent* self = component.GetTransformComponent();
        VelocityComponent* v = component.GetVelocityComponent();
        TransformComponent* target = component.GetTargetTransform();
        if (!self || !v || !target) return;

        // Position
        VectorFloat selfPos{self->position.x, self->position.y};
        VectorFloat targetPos{target->position.x, target->position.y};

        // Calculate flee direction (opposite to target)
        VectorFloat toTarget = targetPos - selfPos;
        if (toTarget.Length() == 0) return;  // Avoid division by zero

        VectorFloat fleeDir = (-toTarget).Normalized();

        // Speed depends on movement mode (walk/run)
        float speed = component.GetSpeed();
        v->dx = fleeDir.x * speed;
        v->dy = fleeDir.y * speed;

        // Trigger flee animation if available
        if (auto* anim = component.GetAnimationComponent()) {
            if (anim->stateMachine) {
                anim->stateMachine->currentState = component.IsRunning() ? "Run" : "Walk";
            }
        }
    }
};

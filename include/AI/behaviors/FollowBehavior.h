#pragma once

#include "AI/AIBehavior.h"
#include "AI/AIController.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"

class FollowBehavior : public AIBehavior {
public:
    void UpdateAI(AIController& component, float deltaTime) override {
        // Check state of NPC
        if (component.GetState() != AIState::Follow) return;

        // Get component data
        TransformComponent* self = component.GetTransformComponent();
        VelocityComponent* v = component.GetVelocityComponent();
        TransformComponent* target = component.GetTargetTransform();
        if (!self || !v || !target) return;

        // Position
        VectorFloat selfPos{self->position.x, self->position.y};
        VectorFloat targetPos{target->position.x, target->position.y};

        // Calculate distance between
        VectorFloat toTarget = targetPos - selfPos;
        float distance = toTarget.Length();

        // Desired follow distance
        float desiredDistance = component.GetDesiredDistance();

        if (distance > desiredDistance) {
            VectorFloat dir = toTarget.Normalized();

            // If it's far -> RUN
            if (distance > desiredDistance * 2.0f) {
                component.SetMovementMode(MovementMode::Run);
            } else {
                component.SetMovementMode(MovementMode::Walk);
            }

            float speed = component.GetSpeed();
            v->dx = dir.x * speed;
            v->dy = dir.y * speed;

            // Animation
            if (auto* anim = component.GetAnimationComponent()) {
                if (anim->stateMachine) {
                    anim->stateMachine->currentState = component.IsRunning() ? "Run" : "Walk";
                }
            }
        } else {
            // Stop if it's too close
            v->dx = 0.0f;
            v->dy = 0.0f;
        }
    }
};

#pragma once

#include "AI/AIBehavior.h"
#include "AI/AIController.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"

class FollowBahavior : public AIBehavior {
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

        // Keep distance between
        float desiredDistance = component.GetDesiredDistance();
        if (distance > desiredDistance) {
            VectorFloat dir = toTarget.Normalized();
            v->dx = dir.x * component.GetSpeed();
            v->dy = dir.y * component.GetSpeed();
        } else {
            // Stop if it's too close
            v->dx = 0.0f;
            v->dy = 0.0f;
        }
    }
};

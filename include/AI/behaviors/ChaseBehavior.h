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
        VectorFloat selfPos = {self->position.x, self->position.y};
        VectorFloat targetPos = {target->position.x, target->position.y};

        // Calculate distance between
        VectorFloat toTarget = targetPos - selfPos;
        if (toTarget.Length() == 0) return;

        VectorFloat dir = toTarget.Normalized();
        v->dx = dir.x * component.GetSpeed();
        v->dy = dir.y * component.GetSpeed();
    }
};

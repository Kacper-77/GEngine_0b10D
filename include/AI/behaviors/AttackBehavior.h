#pragma once

#include "AI/AIBehavior.h"
#include "AI/AIController.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"

class AttackBehavior : public AIBehavior {
public:
    void UpdateAI(AIController& component, float deltaTime) override {
        // Check state of NPC
        if (component.GetState() != AIState::Attack) return;

        // Get component data
        TransformComponent* self = component.GetTransformComponent();
        VelocityComponent* v = component.GetVelocityComponent();
        TransformComponent* target = component.GetTargetTransform();
        if (!self || !v || !target) return;

        // NPC doesn't move
        v->dx = 0.0f;
        v->dy = 0.0f;

        // Calculate distance between
        VectorFloat selfPos{static_cast<float>(self->x), static_cast<float>(self->y)};
        VectorFloat targetPos{static_cast<float>(target->x), static_cast<float>(target->y)};
        float distance = (targetPos - selfPos).Length();

        // Check condition and handle attack
        if (distance <= component.GetAttackRange()) {
            // LATER 
        }
    }
};

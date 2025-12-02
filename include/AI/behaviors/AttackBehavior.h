#pragma once

#include "AI/AIBehavior.h"
#include "AI/AIController.h"
#include "event/core/EventBus.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "components/HealthComponent.h"
#include "utils/Vector.h"
#include "event/custom_events/DamageEvent.h"

DamageEvent CreateDamageEvent(AIController& component) {
    DamageEvent damageEvent{component.GetTarget()};
    damageEvent.amount = component.GetDamage();
    damageEvent.type = component.GetAttackType();
    damageEvent.effect = component.GetAttackEffect();
    damageEvent.effectDuration = component.GetAttackEffectDuration();
    damageEvent.isCriticalEnabled = component.GetEnabledCritical();
    damageEvent.criticalChance = component.GetCriticalChance();
    damageEvent.criticalBonus = component.GetCriticalBonus();
    return damageEvent;
}

class AttackBehavior : public AIBehavior {
public:
    AttackBehavior(EventBus& eventBus) : m_eventBus{eventBus} {}

    void UpdateAI(AIController& component, float deltaTime) override {
        // Check state of NPC
        if (component.GetState() != AIState::Attack) return;

        // Cooldown handling
        m_attackTimer -= deltaTime;
        if (m_attackTimer > 0.0f) return;

        // Get component data
        TransformComponent* self = component.GetTransformComponent();
        VelocityComponent* v = component.GetVelocityComponent();
        TransformComponent* target = component.GetTargetTransform();
        HealthComponent* targetHealth = component.GetTargetHealth();
        if (!self || !v || !target || !targetHealth) return;

        // NPC doesn't move while attacking
        v->dx = 0.0f;
        v->dy = 0.0f;

        // Calculate distance
        VectorFloat selfPos{self->position.x, self->position.y};
        VectorFloat targetPos{target->position.x, target->position.y};
        float distance = (targetPos - selfPos).Length();

        // Skip if target is already dead
        if (targetHealth->isDead) return;

        // Attack if in range
        if (distance <= component.GetAttackRange()) {
            // Publish damage event
            DamageEvent dmgEvent = CreateDamageEvent(component);
            m_eventBus.PublishImmediate(dmgEvent);

            // Trigger attack animation
            if (auto* anim = component.GetAnimationComponent()) {
                if (anim->stateMachine) {
                    anim->stateMachine->currentState = "Attack";
                }
            }
            // Reset cooldown
            m_attackTimer = component.GetAttackCooldown();
        }
    }

private:
    EventBus& m_eventBus;
    float m_attackTimer = 0.0f;
};

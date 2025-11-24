#pragma once

#include "AI/AIBehavior.h"
#include "AI/AIController.h"
#include "event/core/EventBus.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"

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

        // Get component data
        TransformComponent* self = component.GetTransformComponent();
        VelocityComponent* v = component.GetVelocityComponent();
        TransformComponent* target = component.GetTargetTransform();
        HealthComponent* targetHealth = component.GetTargetHealth();
        if (!self || !v || !target || !targetHealth) return;

        // NPC doesn't move
        v->dx = 0.0f;
        v->dy = 0.0f;

        // Calculate distance between
        VectorFloat selfPos{static_cast<float>(self->x), static_cast<float>(self->y)};
        VectorFloat targetPos{static_cast<float>(target->x), static_cast<float>(target->y)};
        float distance = (targetPos - selfPos).Length();

        // Check condition and handle attack
        if (targetHealth->isDead == true) return;

        if (distance <= component.GetAttackRange()) {
            // Create event and send it straight to CombatSystem
            DamageEvent dmgEvent = CreateDamageEvent(component);
            m_eventBus.PublishImmediate(dmgEvent);
        }
    }
private:
    EventBus& m_eventBus;
};

#include "systems/CombatSystem.h"

#include <random>
#include <algorithm>

// Helper
inline float RandomFloat(float min = 0.0f, float max = 1.0f) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

CombatSystem::CombatSystem(ComponentStorage<HealthComponent>& health, EventBus& eventBus)
    :   m_health{health}, m_eventBus{eventBus} 
    {
        m_eventBus.Subscribe<DamageEvent>([this](const DamageEvent& e) {
            OnDamageEvent(e);
        });
    }

void CombatSystem::Update(float deltaTime) {
    for (auto& [entity, health] : m_health.GetAll()) {
        if (health.isDead) continue;

        // Base regen
        if (health.regenRate > 0.0f) {
            health.currentHP = std::min(health.maxHP,
                                        health.currentHP + static_cast<int>(health.regenRate * deltaTime));
        }

        // Tick effects
        for (auto it = health.activeEffects.begin(); it != health.activeEffects.end();) {
            it->duration -= deltaTime;

            if (it->duration <= 0.0f) {
                it = health.activeEffects.erase(it);
            } else {
                switch (it->type) {
                    case EffectType::Poison:
                        health.currentHP -= static_cast<int>(it->tickDamage * deltaTime);
                        break;
                    case EffectType::Burn:
                        health.currentHP -= static_cast<int>((it->tickDamage * 1.5f) * deltaTime);
                        break;
                    case EffectType::Regen:
                        health.currentHP = std::min(health.maxHP,
                            health.currentHP + static_cast<int>(it->tickHeal * deltaTime));
                        break;
                    case EffectType::Stun:
                        // LATER
                        break;
                    default:
                        break;
                }
                ++it;
            }
        }

        // Handle death if necessary
        if (health.currentHP <= 0) {
            health.currentHP = 0;
            health.isDead = true;
        }
    }
}

void CombatSystem::OnDamageEvent(const DamageEvent& e) {
    // Get target HealthComponent and check condition
    if (!e.dest.has_value()) return;
    auto* targetHealth = m_health.Get(*e.dest);
    if (!targetHealth || targetHealth->isDead) return;

    // Original amount of damage
    int originalDamage = e.amount;

    // Handle critical strikes
    if (e.isCriticalEnabled) {
        float roll = RandomFloat();  // Get number between 0.0 - 1.0
        if (roll < e.criticalChance) {
            originalDamage = static_cast<int>(originalDamage * e.criticalBonus);
        }
    }

    // Handle shield
    if (targetHealth->shield > 0) {
        int absorbed = std::min(targetHealth->shield, originalDamage);
        targetHealth->shield -= absorbed;  // Update shield
        originalDamage -= absorbed;        // Reduce damage
    }

    int finalDamage = originalDamage;

    // Handle HP
    if (finalDamage > 0 && !targetHealth->invulnerable) {
        targetHealth->currentHP -= finalDamage;

        if (targetHealth->currentHP <= 0) {
            targetHealth->currentHP = 0;
            targetHealth->isDead = true;
        }
    }
}

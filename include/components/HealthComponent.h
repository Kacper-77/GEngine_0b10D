#pragma once
#include <vector>
#include <string>

enum class EffectType {
    None,
    Poison,
    Burn,
    Regen,
    Stun
};

struct StatusEffect {
    EffectType type = EffectType::None;
    float duration = 0.0f;
    float tickDamage = 0.0f;    
    float tickHeal = 0.0f;      
};

struct HealthComponent {
    int currentHP = 100;
    int maxHP = 100;        
    bool isDead = false;
    int shield = 0;      
    float regenRate = 0.0f;
    bool invulnerable = false;

    std::vector<StatusEffect> activeEffects; // List of all effects

    HealthComponent(int health, int maxHealth)
        : currentHP{health}, maxHP{maxHealth} {}
};

# Health Component 🏥

The **HealthComponent** stores all health‑related data for an entity, including hit points, shields, regeneration, invulnerability, and active status effects.  
It is used by combat systems, AI behaviors, and gameplay logic to manage damage, healing, and temporary effects such as poison or stun.

This component is lightweight but expressive, supporting both simple and advanced combat mechanics.

---

## Overview

The component tracks:

- Current and maximum health
- Death state
- Shield value
- Regeneration rate
- Invulnerability flag
- Active status effects (poison, burn, regen, stun, etc.)

It is designed to be updated by systems such as:

- DamageSystem  
- StatusEffectSystem  
- CombatSystem  
- AISystem  

---

## Component Structure

```cpp
struct HealthComponent {
    int currentHP = 100;
    int maxHP = 100;
    bool isDead = false;
    int shield = 0;
    float regenRate = 0.0f;
    bool invulnerable = false;

    std::vector<StatusEffect> activeEffects;

    HealthComponent(int health, int maxHealth)
        : currentHP{health}, maxHP{maxHealth} {}
};
```

`currentHP / maxHP`
The entity’s current and maximum hit points. When currentHP <= 0, the entity is considered dead.

`isDead`
A cached flag used by systems to avoid repeated death processing.

`shield`
Absorbs incoming damage before HP is reduced. Useful for armor, magical shields, temporary buffs, etc.

`regenRate`
Amount of health regenerated per second. Handled by a regeneration or health update system.

`invulnerable`
When true, the entity ignores all incoming damage. Used for:
- spawn protection
- dodge frames
- scripted events
- boss phases

---

## Status Effects
The component supports multiple simultaneous effects through the StatusEffect struct.

```cpp
struct StatusEffect {
    EffectType type = EffectType::None;
    float duration = 0.0f;
    float tickDamage = 0.0f;
    float tickHeal = 0.0f;
};
```

---

## Why This Design Is Strong
- Modular: status effects are data‑driven and easy to extend

- Flexible: supports shields, regen, invulnerability, and DOT/HOT

- ECS‑friendly: no logic inside the component

- Scalable: multiple effects can stack or coexist

- Clean: simple structure, easy to integrate with systems

---

Summary
The HealthComponent provides:
- Hit point tracking
- Shield and regeneration support
- Invulnerability handling
- A flexible status effect system
- Clean integration with combat and AI systems

It forms the foundation of your engine’s health and damage pipeline.
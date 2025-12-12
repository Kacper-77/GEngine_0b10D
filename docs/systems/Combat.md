# Combat System 🤺

The **CombatSystem** handles all combat‑related logic in the engine, including damage processing, health regeneration, status effects, shields, critical strikes, and death state management.  
It is fully event‑driven and integrates cleanly with ECS through `HealthComponent` and the global `EventBus`.

---

## Overview

The system listens for `DamageEvent` messages and applies damage to the appropriate entity.  
During each update, it also processes:

- Health regeneration  
- Time‑based status effects (poison, burn, regen, stun)  
- Death detection  
- Shield absorption  
- Critical hit calculations  

This design keeps combat logic centralized, deterministic, and easy to extend.

---

## Responsibilities

- Subscribe to and handle `DamageEvent` messages.
- Apply damage, shield absorption, and critical strike modifiers.
- Update health regeneration over time.
- Tick and resolve status effects.
- Mark entities as dead when HP reaches zero.
- Prevent damage to invulnerable entities.
- Cleanly integrate with ECS through `HealthComponent`.

---

## Event‑Driven Damage Handling

The system registers a listener:
```m_eventBus.Subscribe<DamageEvent>(...)```

Whenever a `DamageEvent` is published, the system:

1. Validates the target entity.
2. Applies critical strike logic (if enabled).
3. Applies shield absorption.
4. Applies final damage to HP (unless invulnerable).
5. Marks the entity as dead if HP reaches zero.

This decouples combat from gameplay logic — any system can publish damage events without knowing how damage is processed.

---

## Critical Strikes

If critical hits are enabled:

- A random float in `[0, 1]` is rolled.
- If the roll is below `criticalChance`, damage is multiplied by `criticalBonus`.

This allows for RPG‑style combat mechanics.

---

## Shield Absorption

Before HP is reduced, the system checks:
```absorbed = min(shield, damage)```
```shield -= absorbed```
```damage -= absorbed```

Shields act as temporary HP that fully absorb incoming damage until depleted.

---

## Status Effects

Each entity can have multiple active effects, each with:

- `type` (Poison, Burn, Regen, Stun, etc.)
- `duration`
- `tickDamage` or `tickHeal`

Effects are updated every frame:

### Poison  
Deals damage over time.

### Burn  
Deals increased damage over time (1.5× multiplier).

### Regen  
Heals over time.

### Stun  
Reserved for future logic (movement/attack disabling).

Effects expire when their duration reaches zero.

---

## Health Regeneration

If an entity has a positive `regenRate`, it heals each frame:
```currentHP += regenRate * deltaTime```

Clamped to `maxHP`.

---

## Death Handling

If HP reaches zero:

- `currentHP` is clamped to 0
- `isDead` is set to true

Dead entities no longer regenerate or process effects.

---

## Why This Design Is Strong

- **Event‑driven**: Damage is decoupled from gameplay logic.
- **Modular**: Only entities with `HealthComponent` are processed.
- **Extensible**: New effects or damage types can be added easily.
- **Deterministic**: All combat logic is centralized in one system.
- **ECS‑friendly**: No global state, no cross‑system dependencies.
- **Game‑ready**: Supports crits, shields, regen, DOTs, HOTs, and death.

This is the kind of combat architecture used in RPGs, action games, and roguelikes.

---

## Summary

The **CombatSystem** provides a complete, extensible combat pipeline with:

- Event‑driven damage  
- Critical hits  
- Shields  
- Regeneration  
- Status effects  
- Death state management  

It keeps combat logic clean, scalable, and easy to integrate with other systems.

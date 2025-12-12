# Collider Component 🥊

The **ColliderComponent** defines the collision bounds and collision filtering rules for an entity.  
It is used by the `CollisionSystem` to determine which entities can collide and whether their bounding boxes should be checked for overlap.

This component is intentionally lightweight and optimized for fast AABB (axis‑aligned bounding box) collision detection.

---

## Overview

A collider consists of:

- **Width & height** — the size of the collision box  
- **Collision layer** — what the entity *is*  
- **Collision mask** — what the entity *can collide with*  

This enables flexible collision filtering without complex physics layers.

---

## Component Structure

```cpp
struct ColliderComponent {
    int width, height;
    CollisionLayer layer;  // What this entity is
    CollisionLayer mask;   // What this entity can collide with
};

Width & Height - Define the size of the AABB. The collider is centered around the entity’s transform position.
Collision Layer - Represents the category of the entity (e.g., Player, Enemy, Terrain, Projectile).
Collision Mask - Defines which layers this entity is allowed to collide with.

This allows rules such as:

- Player collides with terrain and enemies
- Enemy collides with player but not with other enemies
- Projectiles collide with enemies but not with the shooter
- Triggers collide only with specific layers

---

## Why This Design Is Strong
Fast: bitmask checks are extremely cheap

Flexible: supports complex collision rules

Simple: no heavy physics layers or tags

Deterministic: symmetric filtering avoids ambiguity

ECS‑friendly: small, cache‑friendly component

This is the same approach used in many professional 2D engines and physics frameworks.

---

## Summary
The ColliderComponent provides:

AABB collision bounds

Layer‑based collision categorization

Mask‑based collision filtering

A fast, symmetric `CanCollide()` helper

It integrates seamlessly with the CollisionSystem and forms the foundation of the engine’s collision pipeline.
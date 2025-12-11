# Movement System 🥾

The **MovementSystem** is responsible for updating entity positions based on their velocity and acceleration.  
It provides a lightweight, ECS‑friendly movement pipeline that handles only kinematic motion, leaving physics and collision resolution to other systems.

---

## Overview

The system processes all entities that contain a `VelocityComponent`.  
For each such entity, it:

1. Applies acceleration (if present) to update velocity.
2. Applies velocity to update position.
3. Skips entities that use the physics system (`PhysicsComponent`), ensuring clean separation between kinematic and physics‑based movement.

This keeps the movement logic simple, deterministic, and modular.

---

## Responsibilities

- Update velocity using acceleration.
- Update position using velocity.
- Ignore entities controlled by the physics system.
- Integrate cleanly with ECS through component storage.

---

## How It Works

### 1. Skip Physics‑Controlled Entities

If an entity has a `PhysicsComponent`, the system does nothing:
```if (m_physics.Has(id)) continue;```

This prevents conflicts between kinematic movement and physics simulation.

### 2. Apply Acceleration

If the entity has an `AccelerationComponent`, velocity is updated:
velocity.dx += acceleration.ax * deltaTime
velocity.dy += acceleration.ay * deltaTime

This allows for:

- Gravity (if handled here)
- Boosts
- Smooth acceleration/deceleration
- Custom movement curves

### 3. Apply Velocity to Position

If the entity has a `TransformComponent`, its position is updated:
```position.x += velocity.dx * deltaTime```
```position.y += velocity.dy * deltaTime```


This is pure kinematic motion — no collision checks, no physics forces.

---

## Why This Design Is Strong

- **Modular**: Movement is separate from physics and collision.
- **Predictable**: Pure kinematic updates, no hidden side effects.
- **ECS‑friendly**: Operates only on required components.
- **Lightweight**: O(n) iteration with minimal branching.
- **Extensible**: Easy to add friction, drag, or custom movement logic.

This is the exact movement architecture used in many 2D engines and ECS frameworks.

---

## Example Use Cases

- UI elements or particles that move without physics.
- Simple enemies or projectiles.
- Camera targets or scripted objects.
- Entities that accelerate smoothly over time.

---

## Summary

The **MovementSystem** provides a clean and efficient kinematic movement pipeline by:

- Applying acceleration  
- Updating velocity  
- Updating position  
- Skipping physics‑controlled entities  

It is simple, scalable, and integrates seamlessly with the rest of the engine.

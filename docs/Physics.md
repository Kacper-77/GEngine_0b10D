# Physics System 👨🏼‍🔬

The **PhysicsSystem** simulates rigid‑body motion for entities using forces, impulses, gravity, friction, damping, and velocity integration.  
It provides a lightweight but expressive physics pipeline suitable for 2D platformers, action games, and arcade‑style movement.

The system updates all entities with a `PhysicsComponent`, applying physical rules and integrating their positions each frame.

---

## Overview

The physics pipeline includes:

- Acceleration from external sources  
- Gravity scaled per entity  
- Impulses (instant velocity changes)  
- Continuous forces (F = m * a)  
- Static and kinetic friction  
- Linear damping (drag)  
- Maximum speed clamping  
- Position integration  
- Grounded state reset (to be set by the CollisionSystem)

This design cleanly separates physics simulation from collision detection and resolution.

---

## Responsibilities

- Apply acceleration to velocity.
- Apply gravity when the entity is not grounded.
- Apply impulses (one‑frame velocity changes).
- Apply continuous forces.
- Apply friction when grounded.
- Apply linear damping.
- Clamp velocity to a maximum speed.
- Integrate velocity into position.
- Reset grounded state for the next frame.

---

## Physics Pipeline Breakdown

### 1. Acceleration

If the entity has an `AccelerationComponent` and is dynamic (`invMass > 0`):
```velocity += acceleration * deltaTime```

This allows external systems to influence physics bodies.

---

### 2. Gravity

If the entity is **not grounded**:
```force.y += gravity * gravityScale * mass```

Gravity is applied as a force, not directly as velocity, keeping the simulation consistent.

---

### 3. Impulses

Impulses represent instant velocity changes (e.g., jumps, explosions):
```velocity += impulse * invMass```
```impulse = {0, 0}```

Impulses are consumed immediately.

---

### 4. Continuous Forces

Forces accumulate over the frame and are converted to acceleration:
```velocity += force * invMass * deltaTime ```
```force = {0, 0}```

This supports:

- pushing  
- pulling  
- wind  
- custom gameplay forces  

---

### 5. Friction (Static + Kinetic)

If the entity is grounded:

- **Static friction** stops very slow movement  
- **Kinetic friction** reduces velocity proportionally to mass and gravity  

```if speed < threshold: velocity = 0``` 
```else: velocity += frictionDir * frictionForce * invMass * deltaTime```

This creates natural sliding and stopping behavior.

---

### 6. Linear Damping (Drag)

Damping reduces velocity uniformly:
```velocity *= (1 - linearDamping)```

Useful for:

- air resistance  
- underwater movement  
- smoothing motion  

---

### 7. Max Speed Clamp

Velocity is clamped to `maxSpeed`:
```if speed > maxSpeed: velocity *= (maxSpeed / speed)```

Prevents unstable or unrealistic acceleration.

---

### 8. Position Integration

Finally, velocity is applied to the transform:
```position += velocity * deltaTime```

This is the core of the physics update.

---

### 9. Grounded Reset

At the end of the frame:
```isGrounded = false```

The `CollisionSystem` will set it again if the entity touches the ground.

This keeps responsibilities cleanly separated.

---

## Gravity Control

Gravity can be configured globally:
```SetGravity(float g),  GetGravity()```

This allows:

- low‑gravity zones  
- underwater areas  
- dynamic gravity changes  

---

## Why This Design Is Strong

- **Modular**: Physics is separate from collision detection.
- **Deterministic**: Forces, impulses, and damping follow predictable rules.
- **Extensible**: Easy to add torque, rotation, or advanced friction models.
- **ECS‑friendly**: Operates only on components, no global state.
- **Game‑ready**: Supports platformers, top‑down games, and action titles.

This is a clean, professional physics architecture.

---

## Summary

The **PhysicsSystem** provides a complete rigid‑body simulation pipeline with:

- Forces  
- Impulses  
- Gravity  
- Friction  
- Damping  
- Speed clamping  
- Position integration  

It is efficient, scalable, and integrates seamlessly with the engine’s ECS and collision systems.

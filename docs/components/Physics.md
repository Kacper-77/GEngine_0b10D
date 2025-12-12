# Physics Component 🌏

The **PhysicsComponent** stores all physical properties required for movement, force accumulation, impulses, friction, gravity scaling, and damping.  
It is used by the `PhysicsSystem` to simulate motion and apply forces in a lightweight 2D physics model.

This component is designed for arcade‑style physics: fast, predictable, and easy to tune.

---

## Overview

The component tracks:

- Mass and inverse mass
- Velocity
- Accumulated forces
- Impulses (instantaneous velocity changes)
- Gravity scaling
- Static and kinetic friction
- Linear damping (air drag)
- Maximum speed
- Grounded state

It enables the physics system to compute acceleration, integrate velocity, and apply friction and drag each frame.

---

## Component Structure

```cpp
struct PhysicsComponent {
    float mass = 1.0f;
    float invMass = 1.0f;

    VectorFloat velocity{0, 0};
    VectorFloat force{0, 0};
    VectorFloat impulse{0, 0};   

    float gravityScale = 1.0f;

    float frictionStatic = 0.6f;
    float frictionKinetic = 0.4f;

    float linearDamping = 0.02f; // drag

    float maxSpeed = 500.0f;

    bool isGrounded = false;

    void SetMass(float m) {
        mass = m;
        invMass = (m == 0.0f ? 0.0f : 1.0f / m);
    }
};
```

---

### Mass & Inverse Mass
`mass`
Defines how strongly forces affect the entity.

`invMass`
Stored as 1 / mass for fast calculations. If mass == 0, the object becomes immovable (infinite mass).

This is a common optimization in physics engines.

### Velocity
Represents the current movement of the entity. Updated each frame based on:
- accumulated forces
- impulses
- gravity
- friction
- damping

### Forces & Impulses
force
Accumulated over the frame using:
```cpp
ApplyForce(...)
```

Used for continuous effects:
- gravity
- wind
- acceleration
- pushing forces

`impulse`
Applied instantly and cleared after integration.

Used for:
- jumps
- knockback
- explosions
- collisions

### Gravity Scale
Allows per‑entity gravity tuning:
- `1.0` -> normal gravity
- `0.0` -> no gravity
- `2.0` -> heavy object
- `-1.0` -> reverse gravity

### Friction
`Static Friction` (frictionStatic)
Resists initial movement when the entity is grounded.

`Kinetic Friction` (frictionKinetic)
Reduces velocity while sliding on a surface.

These values are modified by systems like SurfaceBehaviorSystem.

### Linear Damping (Drag)
Simulates air resistance. Applied every frame to gradually reduce velocity.

### Maximum Speed
Prevents entities from accelerating indefinitely. Useful for:
- player movement
- enemies
- projectiles

### Grounded State
Set by the collision system. Used by physics to:
- allow jumping
- apply friction
- disable gravity when appropriate

---

## Why This Design Is Strong
Fast: minimal data, no heavy physics engine
Flexible: supports forces, impulses, friction, damping
Predictable: arcade‑style physics ideal for 2D games
Modular: integrates cleanly with movement, collision, and surface systems
Extensible: easy to add torque, rotation, or advanced friction later

---

## Summary
The PhysicsComponent provides:
- Mass and inverse mass
- Velocity, forces, and impulses
- Gravity scaling
- Static and kinetic friction
- Linear damping
- Maximum speed limiting
- Grounded state

It forms the core of your engine’s physics simulation and interacts seamlessly with movement, collision, and surface behavior systems.
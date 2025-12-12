# Boundry System 🧱

The **BoundrySystem** enforces world‑space limits for entities by clamping their positions to the screen boundaries and adjusting their physics state accordingly.  
It ensures that entities cannot move outside the visible game area and provides basic collision responses for physics‑enabled objects.

---

## Overview

This system processes all entities that contain a `BoundryComponent`.  
For each such entity, it:

1. Retrieves its `TransformComponent`.
2. Checks whether the entity is allowed to cross each screen edge.
3. Clamps the entity’s position if it violates a boundary.
4. If the entity has a `PhysicsComponent`, it also adjusts velocity and grounded state.

This keeps gameplay objects inside the playable area and prevents physics bodies from escaping the world.

---

## Responsibilities

- Clamp entity positions to the screen rectangle.
- Respect per‑side boundary rules (`blockLeft`, `blockRight`, `blockTop`, `blockBottom`).
- Zero out velocity when hitting a boundary.
- Mark physics bodies as grounded when colliding with the bottom boundary.
- Integrate cleanly with ECS by operating only on relevant components.

---

## How It Works

### 1. Retrieve Screen Size

The system queries the window for:

- `screenWidth`
- `screenHeight`

These values define the world boundaries.

### 2. Clamp Transform Position

For each entity:

- **Left boundary**  
  If `blockLeft` is enabled and `x < 0`, the entity is moved back to `x = 0`.

- **Right boundary**  
  If `blockRight` is enabled and `x + width > screenWidth`, the entity is clamped to the right edge.

- **Top boundary**  
  If `blockTop` is enabled and `y < 0`, the entity is clamped to `y = 0`.

- **Bottom boundary**  
  If `blockBottom` is enabled and `y + height > screenHeight`, the entity is clamped to the bottom edge.

This ensures the entity always stays inside the visible area.

---

## Physics Integration

If the entity also has a `PhysicsComponent`, the system applies simple collision responses:

- Horizontal collisions set `velocity.x = 0`.
- Top collisions set `velocity.y = 0`.
- Bottom collisions:
  - Set `velocity.y = 0`
  - Mark the entity as `isGrounded = true`

This allows physics bodies to:

- Stop when hitting walls
- Land on the ground
- Avoid tunneling outside the screen

The logic is intentionally lightweight and deterministic, making it suitable for platformers, arcade games, and prototypes.

---

## Why This Design Is Strong

- **Simple and predictable**: Boundary checks are explicit and easy to reason about.
- **Modular**: Only entities with `BoundryComponent` are processed.
- **Physics‑aware**: Integrates cleanly with the physics system without requiring a full collision engine.
- **Flexible**: Each side can be enabled or disabled independently.
- **Efficient**: O(n) iteration over boundary components with minimal branching.

This is a clean ECS‑style solution for world limits and basic collision constraints.

---

## Example Use Cases

- Preventing the player from leaving the screen.
- Keeping enemies inside the arena.
- Ensuring physics objects don’t fall out of bounds.
- Creating invisible walls for UI or gameplay zones.

---

## Summary

The **BoundrySystem** enforces screen‑space constraints for entities and provides simple physics collision responses.  
It is lightweight, deterministic, and integrates cleanly with ECS, making it ideal for 2D games that require reliable world boundaries.

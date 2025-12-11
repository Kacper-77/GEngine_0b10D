# Collision System 🏓

The **CollisionSystem** performs broad‑phase and narrow‑phase collision detection using a spatial grid to efficiently reduce the number of entity‑to‑entity checks.  
It integrates with ECS by reading `TransformComponent` and `ColliderComponent` data and outputs a list of collision pairs for other systems (e.g., physics, gameplay logic) to consume.

---

## Overview

The system detects collisions between entities that have both:

- `TransformComponent`
- `ColliderComponent`

It uses a **uniform spatial grid** to partition the world into cells.  
Entities are inserted into all cells they overlap, and only entities sharing a cell (or neighboring cells) are tested for collisions.

This drastically reduces the number of comparisons compared to naive O(n²) checking.

---

## Responsibilities

- Insert collidable entities into a spatial grid.
- Query overlapping and neighboring cells.
- Avoid duplicate collision checks using pair hashing.
- Perform AABB (axis‑aligned bounding box) collision tests.
- Filter collisions using `CanCollide()` rules.
- Store all detected collisions for later processing.

---

## Spatial Grid (Broad‑Phase)

The world is divided into fixed‑size cells (`cellSize`).  
For each entity:

1. Compute which grid cells its bounding box overlaps.
2. Insert the entity ID into those cells.

This allows the system to only check collisions between entities that are spatially close.

### Benefits

- Reduces collision checks from O(n²) to roughly O(n).
- Scales well with large numbers of entities.
- Simple and cache‑friendly.

---

## Collision Detection (Narrow‑Phase)

For each occupied cell:

1. Compare all entity pairs inside the cell.
2. Compare entities with those in neighboring cells.
3. Use a `std::unordered_set` to avoid checking the same pair twice.
4. Perform AABB intersection using:
```!(ax + aw <= bx ||```
  ```ax >= bx + bw ||``` 
  ```ay + ah <= by ||```
  ```ay >= by + bh)```


If the bounding boxes overlap, the pair is added to the collision list.

---

## Collision Filtering

Before performing the AABB test, the system checks:
CanCollide(colliderA, colliderB)


This allows:

- Collision layers
- Masks
- Ignoring certain entity types
- One‑way platforms
- Trigger volumes

This keeps the collision system flexible and gameplay‑friendly.

---

## Output

After the update, the system exposes:
```const std::vector<std::pair<EntityID, EntityID>>& GetCollisions() const```


Other systems (e.g., physics, audio, gameplay events) can consume this list to:

- Resolve physics collisions
- Trigger sound effects
- Spawn particles
- Fire gameplay events

The collision system itself **does not resolve collisions** — it only detects them, keeping responsibilities cleanly separated.

---

## Why This Design Is Strong

- **Efficient**: Spatial partitioning avoids unnecessary checks.
- **Modular**: Only entities with required components are processed.
- **Deterministic**: Pair hashing ensures no duplicates.
- **Scalable**: Works well even with hundreds or thousands of entities.
- **Clean ECS integration**: No global state, no side effects.
- **Extensible**: Collision filtering allows advanced gameplay logic.

This is the same architecture used in many professional 2D engines.

---

## Summary

The **CollisionSystem** provides a fast and scalable collision detection pipeline using:

- Spatial grid broad‑phase  
- AABB narrow‑phase  
- Collision filtering  
- Duplicate‑pair elimination  
- ECS‑friendly data access  

It outputs a clean list of collision pairs for other systems to process, keeping the engine modular and efficient.

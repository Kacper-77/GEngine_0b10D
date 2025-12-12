# Surface Behavior System

The **SurfaceBehaviorSystem** applies movement and physics modifiers based on the type of surface an entity is currently standing on.  
It uses a spatial grid for efficient lookup and supports modifying velocity, friction, and other movement properties depending on the surface material.

This system enables gameplay features such as ice sliding, mud slowing, sand drag, liquid resistance, and more.

---

## Overview

The system processes all entities with a `TransformComponent` and checks whether they overlap with any registered surface areas (`SurfaceComponent`).  
Surfaces are inserted into a spatial grid, allowing fast queries of nearby tiles.

If an entity is inside a surface region, the system applies:

- Velocity multipliers  
- Physics velocity multipliers  
- Friction modifiers  

This allows different terrain types to dynamically affect movement and physics.

---

## Responsibilities

- Insert all surface entities into a spatial grid.
- Query nearby surfaces for each entity.
- Detect whether an entity is inside a surface region.
- Apply velocity multipliers to kinematic movement.
- Apply velocity and friction modifiers to physics bodies.
- Provide configurable default surface speeds.
- Allow custom per‑surface‑type velocity settings.

---

## Spatial Grid Integration

Surfaces are inserted into a uniform spatial grid:
1. for each surface ->
2. compute bounding box ->
3. insert into all overlapping grid cells

This allows the system to efficiently find surfaces near an entity:
```nearby = grid.Query(cellX, cellY)```
```neighbors = grid.QueryNeighbors(cellX, cellY)```

The grid drastically reduces the number of surface checks, making the system scalable even with many surfaces.

---

## Surface Detection

For each entity:

1. Compute its grid cell.
2. Query nearby surfaces.
3. For each candidate surface:
   - Compute surface bounding box.
   - Check if the entity’s position is inside:

```inside = ex >= sx && ex <= sx + sw && ey >= sy && ey <= sy + sh```

4. If inside, apply modifiers and stop checking further surfaces.

This ensures deterministic and efficient behavior.

---

## Movement Modifiers

### Kinematic Movement

If the entity has a `VelocityComponent`:

```velocity.dx *= surface.multiplier```
```velocity.dy *= surface.multiplier```

This affects simple movement systems (non‑physics).

---

### Physics Movement

If the entity has a `PhysicsComponent`:
```phys.velocity.x *= surface.multiplier``` 
```phys.velocity.y *= surface.multiplier```
```phys.frictionKinetic *= surface.frictionMultiplier```

This allows surfaces to:

- Increase or decrease sliding
- Modify friction
- Affect acceleration and deceleration

Examples:

- Ice → low friction, high speed  
- Sand → high friction, low speed  
- Grass → normal movement  
- Liquid → very slow movement  

---

## Default Surface Velocities

The system provides built‑in defaults:

- GRASS = 0.8 
- ICE = 1.05 
- SAND = 0.4 
- LIQUID = 0.2

These can be overridden using:
```SetVelocityBySurfaceType(type, value)```

This allows designers to tune movement behavior without modifying code.

---

## Why This Design Is Strong

- **Efficient**: Spatial grid avoids expensive per‑entity surface scans.
- **Modular**: Surface logic is isolated from movement and physics systems.
- **Flexible**: Supports both kinematic and physics‑based entities.
- **Gameplay‑rich**: Enables varied terrain behavior with minimal code.
- **Extensible**: Easy to add new surface types or modifiers.

This is a professional‑grade approach used in platformers, RPGs, and action games.

---

## Summary

The **SurfaceBehaviorSystem** provides dynamic terrain interaction by:

- Using a spatial grid for fast surface lookup  
- Detecting whether entities stand on specific surfaces  
- Applying velocity and friction modifiers  
- Supporting customizable surface types  

It adds depth and variety to movement and physics while remaining efficient and ECS‑friendly.

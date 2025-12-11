# Animation System  🙆🏼‍♂️

The **AnimationSystem** is responsible for updating and applying both **frame‑based** and **skeletal** animations within the ECS architecture.  
It supports standalone animation layers as well as a full **state machine** with transitions, blending, and per‑layer playback control.

---

## Overview

The system processes all entities that contain an `AnimationComponent`.  
Depending on the component configuration, the system operates in one of two modes:

1. **Simple Layer Mode** – a single animation layer (frame‑based or skeletal).
2. **State Machine Mode** – multiple layers grouped into animation states, with transitions driven by conditions.

This design allows the engine to support everything from simple sprite animations to complex multi‑layer skeletal rigs.

---

## Responsibilities

- Advance animation time based on `deltaTime` and animation speed.
- Loop animations when required.
- Evaluate frame‑based animations and select the correct sprite frame.
- Evaluate skeletal animations by interpolating keyframes.
- Blend skeletal animation transforms with existing world transforms.
- Execute animation state machines:
  - Update all layers in the current state.
  - Evaluate transition conditions.
  - Switch states when conditions are met.
- Apply the resulting animation data to:
  - `SpriteComponent` (for frame animations)
  - `TransformComponent` (for skeletal animations)

---

## How It Works

### 1. Update Loop

For each entity with an `AnimationComponent`:

- If the component contains a **state machine**, the system:
  - Updates all layers in the current state.
  - Evaluates transitions.
  - Applies all layers of the active state.

- Otherwise, the system:
  - Advances the single base layer.
  - Applies it directly.

This dual‑mode approach keeps simple cases lightweight while supporting advanced animation logic when needed.

---

## Frame-Based Animation

Frame animations use a list of frames, each with its own duration and texture.

### Key Features

- Time accumulates until it reaches the duration of the current frame.
- Looping is handled via modulo on total clip duration.
- The system selects the correct frame using `GetFrameIndex()`.
- The selected texture is applied to the entity’s `SpriteComponent`.

This is ideal for classic 2D sprite animations.

---

## Skeletal Animation

Skeletal animations consist of:

- A list of bones (`BoneAnimation`)
- Keyframes per bone
- A global clip duration
- Looping and blending parameters

### Interpolation

The system performs:

- Keyframe lookup
- Time normalization (looping support)
- Linear interpolation (`Lerp`) of:
- Position  
- Scale  
- Rotation  

This produces a smooth `AnimationTransform` for each bone.

### Blending

The system blends the interpolated transform with the entity’s existing `TransformComponent` using a weight factor:

```final = Lerp(baseTransform, animatedTransform, weight)```

This allows:

- Layered skeletal animations  
- Partial overrides  
- Smooth transitions  

---

## Animation State Machine

The state machine supports:

- Multiple states
- Multiple layers per state
- Transitions with custom conditions (function callbacks)
- Per‑state animation blending

### Update Flow

1. Advance all layers in the current state.
2. Check transitions:
   - If a transition’s condition returns true, switch to the target state.
3. Apply all layers of the active state to the entity.

This enables complex behaviors such as:

- Idle -> Walk -> Run  
- Walk -> Jump -> Fall-> Land  
- Attack combos  
- Layered upper‑body animations  

---

## Integration with ECS

The system interacts with:

### `ComponentStorage<AnimationComponent>`
Stores animation data, layers, and state machines.

### `ComponentStorage<SpriteComponent>`
Used for frame‑based animations to update the displayed texture.

### `ComponentStorage<TransformComponent>`
Used for skeletal animations to apply blended bone transforms.
This separation keeps the system modular and consistent with ECS principles.

---

## Why This Design Is Strong

- **Scalable**: Supports simple and advanced animations without code duplication.
- **Modular**: Frame and skeletal animations share a unified layer interface.
- **Performant**: Only updates entities that actually have animation components.
- **Flexible**: State machine allows arbitrary logic and transitions.
- **Blend‑friendly**: Layer weights enable smooth mixing of animations.

This is the kind of architecture used in professional engines.

---

## Example Workflow

1. Entity receives an `AnimationComponent`.
2. Developer assigns:
   - A frame clip, or  
   - A skeletal clip, or  
   - A full state machine with multiple layers.
3. Each frame:
   - System advances animation time.
   - Interpolates or selects frames.
   - Applies results to sprite or transforms.
4. If conditions are met, the state machine transitions to a new state.

---

## Summary

The **AnimationSystem** is a robust, flexible, and scalable animation solution that supports both simple sprite animations and advanced skeletal rigs.  
Its state machine architecture enables expressive character behavior while maintaining clean ECS separation.

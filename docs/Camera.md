# Camera System 🎥

The **CameraSystem** manages all camera behavior in the engine, including following targets, smooth movement, screen shake, zoom control, manual positioning, and world‑space clamping.  
It integrates tightly with ECS by reading `TransformComponent` data and updating the active `CameraComponent`.

---

## Overview

Only one camera can be active at a time.  
The system retrieves the active camera, updates its position based on its mode, applies effects such as shake, and finally sends the camera state to the `RenderSystem`.

Supported camera modes:

- **Static** – fixed position, manual control.
- **Follow** – instantly centers on a target entity.
- **SmoothFollow** – interpolates toward the target for cinematic movement.
- **Shake** – applies randomized offsets for impact effects.

The system also supports:

- Zoom control  
- Screen bounds clamping  
- Manual repositioning  
- Target switching  

---

## Responsibilities

- Track and update the active camera.
- Follow or smoothly follow a target entity.
- Apply camera shake with intensity and duration.
- Clamp camera position to world bounds.
- Allow manual camera control.
- Update zoom and rotation.
- Forward camera state to the `RenderSystem`.

---

## Camera Modes

### **Static**
The camera stays at a fixed position.  
Used when:

- Manually moving the camera
- No target is assigned
- Shake effect ends

### **Follow**
The camera instantly snaps to center the target entity:
camera.position = target.position - viewportSize / 2 + offset

This is ideal for fast‑paced gameplay.

### **SmoothFollow**
The camera interpolates toward the target:
```position += (desired - position) / smoothing```


This creates a cinematic, delayed movement effect.

### **Shake**
Applies random offsets each frame:

- Intensity controls maximum offset
- Duration controls how long the effect lasts
- After finishing, the camera returns to Static mode

Used for:

- Explosions
- Impacts
- Damage feedback

---

## Target Tracking

The camera can lock onto an entity using:
```FocusOn(entityID)```


The system retrieves the entity’s `TransformComponent` and centers the camera on it, optionally applying an offset.

If manual control is enabled or the camera is in Static mode, target tracking is disabled.

---

## Zoom Control

Zoom is clamped to a safe range:
`0.1f ≤ zoom ≤ 5.0f`


Changing zoom automatically switches the camera to:

- **Follow mode** if a target exists  
- **Static mode** if no target is assigned  

This keeps camera behavior predictable.

---

## Screen Shake

Shake is applied by sampling a random offset:

- Using `std::mt19937` for stable randomness
- Range: `[-intensity, +intensity]`
- Applied each frame until duration reaches zero

When shake ends:

- Intensity resets
- Camera returns to Static mode

---

## Bounds Clamping

If enabled, the camera cannot leave a defined world rectangle:

position.x = clamp(position.x, bounds.x, bounds.x + bounds.w - viewportSize.x)
position.y = clamp(position.y, bounds.y, bounds.y + bounds.h - viewportSize.y)


This prevents the camera from showing areas outside the playable world.

---

## Integration with RenderSystem

After updating, the system sends:

- Camera position  
- Zoom  
- Rotation  
- Fade alpha  
- Viewport size  

to the `RenderSystem`, which uses these values to compute the final render transform.

This keeps rendering and camera logic cleanly separated.

---

## Why This Design Is Strong

- **Modular**: Camera logic is fully isolated from rendering.
- **Flexible**: Multiple modes cover most gameplay scenarios.
- **Cinematic**: Smooth follow and shake add polish.
- **ECS‑friendly**: Operates only on components, no global state.
- **Deterministic**: Behavior is predictable and easy to tune.
- **Extensible**: New modes (e.g., rail camera, cutscenes) can be added easily.

This is the kind of camera architecture used in professional 2D engines.

---

## Summary

The **CameraSystem** provides a robust and flexible camera pipeline with:

- Follow and smooth follow modes  
- Manual control  
- Screen shake  
- Zoom and rotation  
- Bounds clamping  
- Clean integration with ECS and rendering  

It enables polished, responsive, and cinematic camera behavior suitable for a wide range of 2D games.

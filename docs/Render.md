# Render System 🎞️

The **RenderSystem** is responsible for drawing all visible entities and background layers to the screen.  
It integrates with ECS by reading `TransformComponent` and `SpriteComponent` data, and uses an abstract `IRenderer` interface to remain backend‑agnostic.

The system supports:

- Camera position and zoom  
- Parallax background layers  
- Viewport‑aware rendering  
- Sprite rendering with world‑space transforms  

---

## Overview

The system iterates over all entities that have a `SpriteComponent` and a `TransformComponent`.  
For each entity, it computes the final screen‑space rectangle based on:

- World position  
- Sprite size  
- Camera position  
- Camera zoom  

It then delegates the actual drawing to the renderer.

Additionally, the system supports multi‑layer parallax backgrounds that scroll relative to the camera.

---

## Responsibilities

- Render all sprites in the scene.
- Apply camera transformations (position, zoom).
- Render parallax background layers.
- Manage viewport size.
- Provide camera data to other systems (e.g., debug overlays).
- Remain decoupled from SDL via the `IRenderer` abstraction.

---

## Sprite Rendering

For each sprite:

1. Retrieve its transform.
2. Compute the destination rectangle:
```dst.x = (worldX - spriteWidth/2 - cameraX) * zoom dst.y = (worldY - spriteHeight/2 - cameraY) * zoom dst.w = spriteWidth * zoom dst.h = spriteHeight * zoom```

3. Draw the texture using the renderer.

This ensures:

- Camera movement shifts the entire world.
- Zoom scales all sprites uniformly.
- Sprites are centered on their transform position.

---

## Background Layers (Parallax)

The system supports multiple background layers, each with:

- A texture  
- A parallax factor  
- Cached texture dimensions  

### How parallax works

For each layer:

- The camera position is multiplied by the layer’s parallax factor.
- The texture is tiled across the viewport.
- Offsets are normalized using modulo to create seamless scrolling.

This allows:

- Multi‑layer parallax backgrounds  
- Infinite scrolling skies  
- Depth illusion in 2D scenes  

Example:

- `parallaxFactor = 0.0` → static background  
- `parallaxFactor = 0.5` → mid‑ground  
- `parallaxFactor = 1.0` → foreground  

---

## Camera Integration

The system exposes setters for:

- `SetCameraPosition`
- `SetCameraZoom`
- `SetCameraRotation` (future use)
- `SetFadeAlpha` (future use)
- `SetViewportSize`

The camera affects all rendering:

- Position offsets world coordinates.
- Zoom scales all sprites and backgrounds.
- Viewport determines how many tiles are needed for parallax layers.

This keeps camera logic separate from rendering logic while still allowing the renderer to use camera data.

---

## Viewport Handling

The viewport defines the visible area in pixels.  
Background layers use it to determine how many tiles to draw:
```for (y = baseOffsetY; y < viewportHeight; y += texH) for (x = baseOffsetX; x < viewportWidth; x += texW)```

This ensures full coverage regardless of camera movement.

---

## Why This Design Is Strong

- **Modular**: Rendering is fully separated from ECS and camera logic.
- **Backend‑agnostic**: Uses `IRenderer`, not SDL directly.
- **Efficient**: Only draws visible sprites; background layers are tiled intelligently.
- **Scalable**: Supports multiple parallax layers with minimal overhead.
- **Clean math**: Camera and zoom are applied consistently across all draw calls.
- **Extensible**: Easy to add rotation, shaders, batching, or sorting.

This is a professional‑grade rendering architecture for 2D engines.

---

## Summary

The **RenderSystem** provides a complete rendering pipeline with:

- Sprite rendering  
- Camera transforms  
- Zoom support  
- Parallax backgrounds  
- Viewport‑aware tiling  
- Clean ECS integration  

It is efficient, flexible, and designed for real‑time 2D games.

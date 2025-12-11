# Audio System 📣

The **AudioSystem** provides a flexible, layered, and entity‑aware audio pipeline built on top of SDL_mixer.  
It supports global sounds, entity‑bound sounds, spatial audio, layered volume control, and per‑entity sound queues.  
This design allows the engine to handle UI audio, gameplay effects, ambient loops, and music in a clean and scalable way.

---

## Overview

The system manages all audio playback in the engine.  
It supports three major categories of sounds:

1. **Entity‑bound sounds** – effects tied to specific entities (e.g., footsteps, collisions).
2. **Global sounds** – music or ambient effects not tied to any entity.
3. **Layered sounds** – audio grouped into logical layers (UI, Gameplay, Ambient, Music) with independent volume/mute control.

The system integrates with ECS by storing audio mappings per entity and cleaning up automatically when entities are destroyed.

---

## Responsibilities

- Play and stop entity‑specific sounds.
- Play and stop global sounds (music, ambient loops).
- Manage layered audio with independent volume and mute states.
- Handle spatial audio with stereo panning based on listener position.
- Maintain per‑entity sound queues for sequential playback.
- Clean up audio data for destroyed entities.
- Convert between integer and float vector formats for spatial calculations.

---

## Entity‑Bound Audio

Each entity can register multiple sounds using:
RegisterSound(entityID, tag, audio)

Playback is triggered via:
```Play(entityID, tag)``` 
```Stop(entityID, tag)```


The system stores the SDL_mixer channel used for each sound, allowing precise control and cleanup.

---

## Global Audio

Global sounds are not tied to any entity and are typically used for:

- Background music
- Ambient loops
- UI themes

They are registered with:
RegisterGlobalSound(tag, audio)

And played using:
```PlayGlobal(tag)``` 
```StopGlobal(tag)```


Music uses `Mix_PlayMusic`, while sound effects use `Mix_PlayChannel`.

---

## Layered Audio

The system supports four logical audio layers:

- **UI**
- **Gameplay**
- **Ambient**
- **Music**

Each layer has:

- Independent volume (`SetLayerVolume`)
- Independent mute state (`MuteLayer`, `UnmuteLayer`)

This allows the engine to implement:

- Master volume sliders
- Mute UI sounds while keeping music
- Lower ambient volume dynamically
- Per‑layer audio balancing

Layered sounds are registered with:
- RegisterLayeredSound(tag, audio, layer)

---

## Spatial Audio

The system provides simple but effective 2D spatial audio using stereo panning.

### How it works

1. The system retrieves the entity’s position.
2. It compares it to the listener’s position.
3. It computes a left/right pan value in the range `[-1, 1]`.
4. It converts this into SDL_mixer left/right channel volumes.
5. It plays the sound with `Mix_SetPanning`.

This allows effects like:

- Footsteps moving left/right
- Passing objects
- Directional ambient sources

---

## Sound Queues

Each entity can enqueue sounds to be played sequentially:
- EnqueueSound(entityID, audio)


The system ensures:

- Only one sound plays at a time per entity.
- Music and chunks are both supported.
- Playback continues automatically when the previous sound finishes.

This is useful for:

- Combo attack sounds
- Dialogue lines
- Sequential UI notifications

---

## Update Loop

During each update:

1. The system checks if an entity’s queue is currently playing.
2. If the current sound finished, it dequeues the next one.
3. It plays the next sound (music or chunk).
4. It marks the queue as active until playback completes.

This keeps audio sequencing deterministic and entity‑scoped.

---

## Cleanup

When an entity is destroyed, the system removes:

- Registered sounds
- Spatial positions
- Sound queues
- Active channels

This prevents dangling audio references and ensures memory safety.

---

## Why This Design Is Strong

- **Layered audio** mirrors professional engines (Unity, Unreal).
- **Entity‑bound playback** integrates cleanly with ECS.
- **Spatial audio** adds immersion with minimal overhead.
- **Sound queues** allow complex audio behavior without extra systems.
- **Automatic cleanup** prevents leaks and dangling channels.
- **SDL_mixer abstraction** keeps the engine portable and modular.

The system is flexible enough for small prototypes but structured like a real game engine audio pipeline.

---

## Summary

The **AudioSystem** is a robust, modular, and ECS‑friendly audio solution that supports:

- Entity‑based effects  
- Global music and ambient audio  
- Layered volume control  
- Spatial stereo panning  
- Per‑entity sound queues  
- Automatic cleanup  

It provides a clean abstraction over SDL_mixer while enabling scalable audio behavior for gameplay, UI, and ambient environments.

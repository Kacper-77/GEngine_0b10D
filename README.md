# Project Overview

I started this project out of pure curiosity.  
I wanted to understand how a real game engine works under the hood — not by reading about it, but by building one piece by piece.  
Along the way I realized how much more there is to explore, how many systems could still be improved, and how far this project could grow if I kept pushing it.  
But at some point you have to move forward, learn new things, and come back only when you’re ready to take the next step.  
This engine represents that journey.

---

## What This Project Is

This repository contains a fully modular, data‑driven **2D game engine** built from scratch in C++ using an **Entity‑Component‑System (ECS)** architecture.  
It is not a framework glued together from tutorials — it is a complete, custom engine with real gameplay systems, rendering, physics, AI, audio, and world management.

The goal was to create a clean, understandable, extensible architecture that mirrors how professional engines structure their internal systems.

---

## Core Architecture

### ✅ Entity‑Component‑System (ECS)
- **Entities** are simple IDs.
- **Components** store pure data (Transform, Physics, Collider, Sprite, Health, Animation, AI, Surface, etc.).
- **Systems** operate on component sets and implement all gameplay logic.

This keeps the engine modular, cache‑friendly, and easy to extend.

---

## World & Core Managers

The `World` class acts as the central hub of the engine:

- Registers and stores **ComponentStorage<T>**
- Manages **systems** (Movement, Physics, Collision, Render, Audio, Camera, SurfaceBehavior, etc.)
- Provides access to:
  - `EntityManager`
  - `SystemManager`
  - `EventBus`
  - `LevelManager`
  - `InputManager`

Everything is decoupled and replaceable — the world only knows interfaces, not implementations.

---

## Systems Included

### ✅ Rendering System  
Draws sprites, animations, parallax layers, and debug overlays.

### ✅ Physics System  
Applies forces, impulses, gravity, friction, damping, and integrates velocity.

### ✅ Collision System  
AABB collision detection with layer/mask filtering and spatial partitioning.

### ✅ Movement System  
Handles kinematic movement for entities without physics.

### ✅ Surface Behavior System  
Applies terrain modifiers (ice, sand, liquid, grass) to velocity and friction.

### ✅ Camera System  
Smooth following, clamping, zoom, shake, and viewport management.

### ✅ Audio System  
Plays music, ambient sounds, and effects with volume control.

### ✅ Entity Creation System  
Spawns entities, assigns components, and handles initialization logic.

### ✅ Boundary System  
Prevents entities from leaving the playable area.

### ✅ AI System  
Modular AI controllers with pluggable behaviors, perception, combat stats, and state machines.

---

## Component Storage

A generic `ComponentStorage<T>` handles:

- Add / Get / Remove
- Fast lookup via `unordered_map`
- Iteration over all components of a type

This keeps ECS clean and efficient.

---

## Entity Management

`EntityManager` provides:

- Entity lifecycle (alive/dead)
- Tags and groups (e.g., "Enemy", "Player")
- Arbitrary metadata (`AddInfo`)
- Respawn points
- Loot drops

It acts as the engine’s registry and gameplay database.

---

## Why This Project Matters

This engine is not meant to compete with Unity or Unreal.  
It exists because building something from scratch teaches you things no tutorial ever will:

- How systems interact  
- How to design clean architecture  
- How to debug complex behavior  
- How to think like an engine developer  

It’s a foundation — one I can return to, extend, or rebuild entirely with the experience gained.

---

## Final Thoughts

This project represents a chapter of learning, experimentation, and curiosity.  
It’s not perfect, and it’s not finished — but it’s real, functional, and built with intention.  
Maybe one day I’ll come back and push it further.  
For now, it stands as a milestone in my journey as a developer.

---

## More Information & Demo

If you want to dive deeper into how the engine works internally, the full technical documentation is available in the **`docs/`** directory.  
It contains detailed explanations of systems, components, architecture, and design decisions.

A small demonstration of the engine in action can be found in the **`demo/`** folder, including short recordings showing core features such as movement, physics, AI, collision, and rendering.

These resources should give you a clear picture of how the engine behaves in practice and how all the systems come together.

# AI System 🤖

The **AI System** manages all AI controllers in the engine and updates their behavior each frame.  
It provides a flexible, modular architecture where each AI entity is driven by an `AIController` and a pluggable `AIBehavior` implementation.

This design allows for simple state machines, complex behavior trees, or custom logic without modifying the core engine.

---

## Overview

The AI system consists of three main parts:

- **AISystem** – a manager that stores and updates all AI controllers  
- **AIController** – a data‑rich component representing an AI agent  
- **AIBehavior** – an abstract interface for defining custom AI logic  

This separation keeps AI logic clean, testable, and easy to extend.

---

# AISystem

The `AISystem` stores all active AI controllers and updates them every frame.

### Responsibilities

- Register and remove AI controllers  
- Assign unique controller IDs  
- Update all controllers each frame  
- Serve as the central AI manager  

### Key Methods

- `AddController(AIController* controller)`  
- `RemoveController(ControllerID id)`  
- `GetController(ControllerID id)`  
- `Update(float deltaTime)`  

The system does **not** own the controllers — it simply manages and updates them.

---

# AIController

The `AIController` is the core of the AI agent.  
It stores:

- **Stats** (health, armor, stamina, morale, damage, speed)  
- **Perception** (vision range, hearing range, field of view)  
- **Combat data** (attack range, cooldowns, crit chance, effects)  
- **Movement data** (position, velocity, patrol routes, movement mode)  
- **Targeting** (current target entity, target transform, target health)  
- **State machine** (Idle, Patrol, Attack, Flee, Chase, Follow, Dead)  
- **Faction and friendliness**  
- **Animation component**  
- **Current AI behavior**  

It acts as a **blackboard** for AI logic — a central place where behavior scripts read and write data.

---

## State Machine

The controller exposes a simple state enum:
```Idle, Patrol, Attack, Flee, Chase, Follow, Dead```

States can be changed via:
```ChangeState(AIState newState)```

This allows behaviors to implement:

- patrol loops  
- chase logic  
- flee logic  
- attack sequences  
- idle wandering  
- death handling  

---

## Perception

The controller supports basic perception:

- `visionRange`
- `hearingRange`
- `fieldOfView`

With helper methods:

- `CanSeeEnemy()`
- `CanHearEnemy()`
- `HealthLow()`

These can be expanded into full perception systems later.

---

## Movement

The controller stores:

- current position  
- velocity  
- destination  
- patrol route  
- movement mode (Walk / Run)  
- walk and run speeds  

It can attach to ECS components:
```AttachComponents(TransformComponent, VelocityComponent)```

This allows AI to directly influence movement systems.

---

## Combat

The controller supports:

- attack range  
- damage  
- armor  
- critical hits  
- attack effects (poison, burn, etc.)  
- attack cooldown  
- custom cooldowns  

Helper methods:

- `CanAttack()`
- `TickAttackTimer(dt)`
- `ResetAttackTimer()`

This integrates cleanly with your CombatSystem.

---

## Targeting

The controller can track:

- target entity ID  
- target transform  
- target health  

Methods:

- `SetTarget(EntityID)`
- `ClearTarget()`
- `GetTarget()`

This enables chase, follow, attack, and flee behaviors.

---

## Behavior Attachment

The controller holds a `std::unique_ptr<AIBehavior>`:
```void SetBehavior(std::unique_ptr<AIBehavior> newBehavior)```

This allows:

- finite state machines  
- behavior trees  
- utility AI  
- scripted behaviors  

The controller does not care *how* behavior is implemented — it only calls:
```behavior->UpdateAI(*this, deltaTime)```

---

# AIBehavior

`AIBehavior` is a simple abstract interface

This makes it easy to create custom behaviors:

- PatrolBehavior  
- ChaseBehavior  
- AttackBehavior  
- FleeBehavior  
- BossBehavior  
- CompanionFollowBehavior  

Each behavior receives full access to the AIController, allowing it to:

- read stats  
- change state  
- move the agent  
- attack  
- pick targets  
- modify animations  

---

# Summary

The AI system provides a flexible, modular architecture with:

- A central AISystem manager  
- A powerful AIController acting as a blackboard  
- A pluggable AIBehavior interface  
- Built‑in support for perception, movement, combat, targeting, and states  

This design is scalable, extensible, and suitable for anything from simple enemies to complex boss logic.

#include "systems/MovementSystem.h"
#include <iostream>

MovementSystem::MovementSystem(ComponentStorage<TransformComponent>& transforms,
                               ComponentStorage<VelocityComponent>& velocities,
                               ComponentStorage<AccelerationComponent>& accelerations,
                               ComponentStorage<PhysicsComponent>& physics)
    : m_transforms{transforms}, m_velocities(velocities), 
      m_accelerations(accelerations), m_physics{physics} {}

// Update state
void MovementSystem::Update(float deltaTime) {
    for (auto& [id, velocity] : m_velocities.GetAll()) {
        if (m_physics.Has(id)) continue;
        
        // Get key info from components
        auto* transform = m_transforms.Get(id);
        auto* acceleration = m_accelerations.Get(id);
        
        // Check conditions and set values
        if (acceleration) {
            velocity.dx += acceleration->ax * deltaTime;
            velocity.dy += acceleration->ay * deltaTime;
        }

        if (transform) {
            transform->position.x += velocity.dx * deltaTime;
            transform->position.y += velocity.dy * deltaTime;
        }
    }
}

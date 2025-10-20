#include "systems/MovementSystem.h"
#include <iostream>

MovementSystem::MovementSystem(ComponentStorage<TransformComponent>& transforms,
                               ComponentStorage<VelocityComponent>& velocities,
                               ComponentStorage<AccelerationComponent>& accelerations)
    : m_transforms{transforms}, m_velocities(velocities), m_accelerations(accelerations) {}

    // Update state
void MovementSystem::Update(float deltaTime) {
    for (auto& [id, velocity] : m_velocities.GetAll()) {
        // Get key info from components
        auto* transform = m_transforms.Get(id);
        auto* acceleration = m_accelerations.Get(id);
        
        // Check conditions and set values
        if (acceleration) {
            velocity.dx += acceleration->ax * deltaTime;
            velocity.dy += acceleration->ay * deltaTime;
        }

        if (transform) {
            transform->x += static_cast<int>(velocity.dx * deltaTime);
            transform->y += static_cast<int>(velocity.dy * deltaTime);
        }
    }
}

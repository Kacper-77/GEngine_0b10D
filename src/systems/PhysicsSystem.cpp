#include "systems/PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(ComponentStorage<TransformComponent>& transforms,
                             ComponentStorage<AccelerationComponent>& accelerations,
                             ComponentStorage<ForceComponent>& forces,
                             ComponentStorage<PhysicsComponent>& physics)
    : m_transforms{transforms}, m_accelerations{accelerations}, 
      m_forces{forces}, m_physics{physics} {}

// Update state
void PhysicsSystem::Update(float deltaTime) {
    const float globalGravity = 9.8f;  // WORLD WILL REPLACE IT

    for (auto& [id, physics] : m_physics.GetAll()) {
        auto* acceleration = m_accelerations.Get(id);
        auto* transform = m_transforms.Get(id);
        if (!transform) continue;

        // Apply physical forces
        if (acceleration) {
            physics.velocityX += (acceleration->ax / physics.mass) * deltaTime;
            physics.velocityY += (acceleration->ay / physics.mass) * deltaTime;
            acceleration->ax = 0.0f;
            acceleration->ay = 0.0f;
        }

        auto* force = m_forces.Get(id);
        if (force) {
            physics.velocityX += (force->fx / physics.mass) * deltaTime;
            physics.velocityY += (force->fy / physics.mass) * deltaTime;

            if (!force->persistent) {
                force->fx = 0.0f;
                force->fy = 0.0f;
            }
        }

        // Gravity
        physics.velocityY += globalGravity * physics.gravityScale * deltaTime;

        // Friction
        physics.velocityX *= (1.0f - physics.friction);
        physics.velocityY *= (1.0f - physics.friction);

        // Velocity
        physics.velocityX = std::clamp(physics.velocityX, -physics.maxVelocityX, physics.maxVelocityX);
        physics.velocityY = std::clamp(physics.velocityY, -physics.maxVelocityY, physics.maxVelocityY);

        // 5. Update position
        transform->position.x += physics.velocityX * deltaTime;
        transform->position.y += physics.velocityY * deltaTime;
        physics.posX = transform->position.x;
        physics.posY = transform->position.y;

        physics.isGrounded = false;
    }
}

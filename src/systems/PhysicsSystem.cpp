#include "systems/PhysicsSystem.h"

// UPGRADE AND OPTIMALIZATION IS COMING

PhysicsSystem::PhysicsSystem(ComponentStorage<TransformComponent>& transforms,
                             ComponentStorage<AccelerationComponent>& accelerations,
                             ComponentStorage<PhysicsComponent>& physics)
    : m_transforms{transforms}, m_accelerations{accelerations}, m_physics{physics} {}

// Update state
void PhysicsSystem::Update(float deltaTime) {
    for (auto& [id, physics] : m_physics.GetAll()) {
        auto* acceleration = m_accelerations.Get(id);
        if (std::abs(physics.velocityX) < 0.01f) physics.velocityX = 0.0f;
        if (std::abs(physics.velocityY) < 0.01f) physics.velocityY = 0.0f;
        
        if (acceleration) {
            physics.velocityX += acceleration->ax * deltaTime;
            physics.velocityY += acceleration->ay * deltaTime;

            acceleration->ax = 0.0f;
            acceleration->ay = 0.0f;
        }

        physics.velocityX *= (1.0f - physics.friction);
        physics.velocityY *= (1.0f - physics.friction);
        physics.velocityY += physics.gravity * deltaTime;

        auto* transform = m_transforms.Get(id);
        if (transform) {
            transform->x += physics.velocityX * deltaTime;
            transform->y += physics.velocityY * deltaTime;
            physics.posX += physics.velocityX * deltaTime;
            physics.posY += physics.velocityY * deltaTime;
        }
    }
}
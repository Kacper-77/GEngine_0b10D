#include "systems/PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(ComponentStorage<TransformComponent>& transforms,
                             ComponentStorage<AccelerationComponent>& accelerations,
                             ComponentStorage<PhysicsComponent>& physics)
    : m_transforms{transforms}, m_accelerations{accelerations}, m_physics{physics} {}

// Update state
void PhysicsSystem::Update(float deltaTime) {
    const float GRAVITY = GetGravity();

    for (auto& [id, phys] : m_physics.GetAll()) {
        auto* transform = m_transforms.Get(id);
        if (!transform) continue;

        auto* accel = m_accelerations.Get(id);
        if (accel && phys.invMass > 0.0f) {
            phys.velocity.x += accel->ax * deltaTime;
            phys.velocity.y += accel->ay * deltaTime;
        }

        // Apply gravity
        if (!phys.isGrounded && phys.invMass > 0.0f) {
            phys.force.y += GRAVITY * phys.gravityScale * phys.mass;
        }

        // Apply impulses (instant velocity change)
        if (phys.invMass > 0.0f) {
            phys.velocity.x += phys.impulse.x * phys.invMass;
            phys.velocity.y += phys.impulse.y * phys.invMass;
        }
        phys.impulse = {0, 0};

        // Apply forces (F = m * a → a = F * invMass)
        if (phys.invMass > 0.0f) {
            phys.velocity.x += phys.force.x * phys.invMass * deltaTime;
            phys.velocity.y += phys.force.y * phys.invMass * deltaTime;
        }
        phys.force = {0, 0};

        // Apply friction (static + kinetic)
        if (phys.isGrounded) {
            float speed = std::sqrt(phys.velocity.x * phys.velocity.x +
                                    phys.velocity.y * phys.velocity.y);

            if (speed < 0.01f) {
                phys.velocity = {0, 0}; // static friction stops object
            } else {
                float frictionForce = phys.frictionKinetic * phys.mass * GRAVITY;
                VectorFloat frictionDir = {
                    -phys.velocity.x / speed,
                    -phys.velocity.y / speed
                };

                phys.velocity.x += frictionDir.x * frictionForce * phys.invMass * deltaTime;
                phys.velocity.y += frictionDir.y * frictionForce * phys.invMass * deltaTime;
            }
        }

        // Apply linear damping (drag)
        phys.velocity.x *= (1.0f - phys.linearDamping);
        phys.velocity.y *= (1.0f - phys.linearDamping);

        // Clamp max speed
        float speed = std::sqrt(phys.velocity.x * phys.velocity.x +
                                phys.velocity.y * phys.velocity.y);

        if (speed > phys.maxSpeed) {
            float scale = phys.maxSpeed / speed;
            phys.velocity.x *= scale;
            phys.velocity.y *= scale;
        }

        // Integrate position
        transform->position.x += phys.velocity.x * deltaTime;
        transform->position.y += phys.velocity.y * deltaTime;

        // Reset grounded (CollisionSystem will set it again)
        phys.isGrounded = false;
    }
}

void PhysicsSystem::SetGravity(float gravity) { m_gravity = gravity; }
const float PhysicsSystem::GetGravity() const { return m_gravity; }

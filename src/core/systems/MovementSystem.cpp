#include "core/systems/MovementSystem.h"

MovementSystem::MovementSystem(ComponentStorage<TransformComponent>& transforms,
                               ComponentStorage<VelocityComponent>& velocities)
    : m_transforms(transforms), m_velocities(velocities) {}

void MovementSystem::Update(float deltaTime) {
    for (auto& [id, velocity] : m_velocities.GetAll()) {
        auto* transform = m_transforms.Get(id);
        if (transform) {
            transform->x += static_cast<int>(velocity.dx * deltaTime);
            transform->y += static_cast<int>(velocity.dy * deltaTime);
        }
    }
}

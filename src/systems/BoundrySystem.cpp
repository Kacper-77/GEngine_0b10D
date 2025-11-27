#include "systems/BoundrySystem.h"

BoundrySystem::BoundrySystem(ComponentStorage<TransformComponent>& transforms,
              ComponentStorage<BoundryComponent>& boundaries,
              Window* window)
    : m_transforms{transforms}, m_boundaries{boundaries}, m_window{window} {}

// Update state
void BoundrySystem::Update(float deltaTime) {
    const int screenWidth = m_window->GetWidth();
    const int screenHeight = m_window->GetHeight();

    for (auto& [id, boundry] : m_boundaries.GetAll()) {
        auto* transform = m_transforms.Get(id);
        if (!transform) continue;

        // Left
        if (boundry.blockLeft && transform->position.x < 0.0f) {
            transform->position.x = 0.0f;
        }

        // Right
        if (boundry.blockRight && transform->position.x + transform->scale.x > static_cast<float>(screenWidth)) {
            transform->position.x = static_cast<float>(screenWidth) - transform->scale.x;
        }

        // Up
        if (boundry.blockTop && transform->position.y < 0.0f) {
            transform->position.y = 0.0f;
        }

        // Down
        if (boundry.blockBottom && transform->position.y + transform->scale.y > static_cast<float>(screenHeight)) {
            transform->position.y = static_cast<float>(screenHeight) - transform->scale.y;
        }
    }
}
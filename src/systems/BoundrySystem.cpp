#include "systems/BoundrySystem.h"

BoundrySystem::BoundrySystem(ComponentStorage<TransformComponent>& transforms,
              ComponentStorage<BoundryComponent>& boundries,
              Window* window)
    : m_transforms{transforms}, m_boundries{boundries}, m_window{window} {}

void BoundrySystem::Update(float deltaTime) {
    const int screenWidth = m_window->GetWidth();
    const int screenHeight = m_window->GetHeight();

    for (auto& [id, boundry] : m_boundries.GetAll()) {
        auto* transform = m_transforms.Get(id);
        if (!transform) continue;

        if (boundry.blockLeft && transform->x < 0) {
            transform->x = 0;
        }

        if (boundry.blockRight && transform->x + transform->width > screenWidth) {
            transform->x = screenWidth - transform->width;
        }

        if (boundry.blockTop && transform->y < 0) {
            transform->y = 0;
        }

        if (boundry.blockBottom && transform->y + transform->height > screenHeight) {
            transform->y = screenHeight - transform->height;
        }
    }
}
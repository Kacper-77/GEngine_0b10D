#include "systems/CollisionSystem.h"
#include <iostream>

CollisionSystem::CollisionSystem(EntityManager& entityManager,
                                 ComponentStorage<TransformComponent>& transforms,
                                 ComponentStorage<ColliderComponent>& colliders,
                                 EventBus& eventBus)
        : m_entityManager{entityManager}, 
          m_transforms{transforms}, 
          m_colliders{colliders}, 
          m_eventBus{eventBus} {}

void CollisionSystem::Update(float deltaTime) {
    auto entities = m_entityManager.GetAllEntities();

    for (auto it1 = entities.begin(); it1 != entities.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != entities.end(); ++it2) {
            EntityID a = *it1;
            EntityID b = *it2;

            if (!m_colliders.Has(a) || !m_colliders.Has(b)) continue;
            if (!m_transforms.Has(a) || !m_transforms.Has(b)) continue;

            const auto& ta = *m_transforms.Get(a);
            const auto& tb = *m_transforms.Get(b);

            const auto& ca = *m_colliders.Get(a);
            const auto& cb = *m_colliders.Get(b);

            if (IsColliding(ta.x, ta.y, ca.width, ca.height,
                            tb.x, tb.y, cb.width, cb.height)) {
                HandleCollision(a, b);
            }
        }
    }
}

bool CollisionSystem::IsColliding(int ax, int ay, int aw, int ah,
                                  int bx, int by, int bw, int bh) {
    return !(ax + aw <= bx ||
             ax >= bx + bw ||
             ay + ah <= by ||
             ay >= by + bh);
}

void CollisionSystem::HandleCollision(EntityID a, EntityID b) {
    m_eventBus.Publish(CollisionEvent{a, b});
}
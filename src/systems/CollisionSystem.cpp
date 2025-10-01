#include "systems/CollisionSystem.h"
#include <iostream>

CollisionSystem::CollisionSystem(EntityManager& entityManager,
                                 ComponentStorage<ColliderComponent>& colliders,
                                 EventManager& eventManager,
                                 EventFactory& eventFactory,
                                 EventResolver& eventResolver)
        : m_entityManager(entityManager), 
          m_colliders(colliders), 
          m_eventManager(eventManager), 
          m_eventFactory(eventFactory), m_eventResolver(eventResolver) {}

void CollisionSystem::Update(float deltaTime) {
    auto entities = m_entityManager.GetAllEntities();

    for (auto it1 = entities.begin(); it1 != entities.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != entities.end(); ++it2) {
            EntityID a = *it1;
            EntityID b = *it2;

            if (!m_colliders.Has(a) || !m_colliders.Has(b)) continue;

            const auto& ca = *m_colliders.Get(a);
            const auto& cb = *m_colliders.Get(b);

            if (IsColliding(ca, cb)) {
                std::string eventType = m_eventResolver.ResolveEventType(
                    m_entityManager.GetInfo(a, "behavior"),
                    m_entityManager.GetInfo(b, "behavior"));
                    
                HandleCollision(a, b, eventType);
            }
        }
    }
}

bool CollisionSystem::IsColliding(const ColliderComponent& a, const ColliderComponent& b) {
    return !(a.x + a.width < b.x  ||
             a.x > b.x + b.width  ||
             a.y + a.height < b.y ||
             a.y > b.y + b.height);
}

void CollisionSystem::HandleCollision(EntityID a, EntityID b, const std::string& eventType) {
    Event event = m_eventFactory.Create(eventType, a, b);
    m_eventManager.Emit(event);
}
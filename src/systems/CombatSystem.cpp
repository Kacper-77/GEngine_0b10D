#include "systems/CombatSystem.h"

CombatSystem::CombatSystem(ComponentStorage<HealthComponent>& health, EventBus& eventBus)
    :   m_health{health}, m_eventBus{eventBus} 
    {
        m_eventBus.Subscribe<DamageEvent>([this](const DamageEvent& e) {
            OnDamageEvent(e);
        });
    }

void CombatSystem::Update(float deltaTime) {

}

void CombatSystem::OnDamageEvent(const DamageEvent& e) {
    
}
